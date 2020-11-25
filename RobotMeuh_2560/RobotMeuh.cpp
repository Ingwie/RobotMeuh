/*
.-------.        ,-----.     _______       ,-----.  ,---------. ,---.    ,---.    .-''-.    ___    _ .---.  .---.
|  _ _   \     .'  .-,  '.  \  ____  \   .'  .-,  '.\          \|    \  /    |  .'_ _   \ .'   |  | ||   |  |_ _|
| ( ' )  |    / ,-.|  \ _ \ | |    \ |  / ,-.|  \ _ \`--.  ,---'|  ,  \/  ,  | / ( ` )   '|   .'  | ||   |  ( ' )
|(_ o _) /   ;  \  '_ /  | :| |____/ / ;  \  '_ /  | :  |   \   |  |\_   /|  |. (_ o _)  |.'  '_  | ||   '-(_{;}_)
| (_,_).' __ |  _`,/ \ _/  ||   _ _ '. |  _`,/ \ _/  |  :_ _:   |  _( )_/ |  ||  (_,_)___|'   ( \.-.||      (_,_)
|  |\ \  |  |: (  '\_/ \   ;|  ( ' )  \: (  '\_/ \   ;  (_I_)   | (_ o _) |  |'  \   .---.' (`. _` /|| _ _--.   |
|  | \ `'   / \ `"/  \  ) / | (_{;}_) | \ `"/  \  ) /  (_(=)_)  |  (_,_)  |  | \  `-'    /| (_ (_) _)|( ' ) |   |
|  |  \    /   '. \_/``".'  |  (_,_)  /  '. \_/``".'    (_I_)   |  |      |  |  \       /  \ /  . \ /(_{;}_)|   |
''-'   `'-'      '-----'    /_______.'     '-----'      '---'   '--'      '--'   `'-..-'    ``-'`-'' '(_,_) '---'
*/
/*         Copyright 2020 by Ingwie (Bracame)          */
/*   Licence: GPLV3 see <http://www.gnu.org/licenses   */
/*        Compile with AVR GCC + Code::Blocks          */
/*    https://www.mediafire.com/file/cahqfrm90h7c7fy/  */
/*    Setup_OAVRCBuilder3.exe/file (Pswd : OpenAVRc)   */


#include "RobotMeuh.h"

//ROBOTMEUH
Status_t RobotStatus = {0};
DataLcdToMain_t lcdReport = {0};
SystemBools_t SystemBools = {0};

//SPI (LCD com.)
char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
volatile uint8_t SpiBufNum = 0;

//TIME
time_t rtcTime;
uint8_t counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))

//IMU
FusionBias fusionBias;
FusionAhrs fusionAhrs;
float samplePeriod = 0.01f; // sample period in seconds
FusionVector3 gyroscopeSensitivity = {GYRO_RATE_XYZ, GYRO_RATE_XYZ, GYRO_RATE_XYZ};// sensitivity in degrees per second per lsb
FusionVector3 accelerometerSensitivity = {ACC_RATE_XYZ, ACC_RATE_XYZ, ACC_RATE_XYZ}; // Sensitivity in g per lsb
FusionVector3 hardIronBias = {0.0f, 0.0f, 0.0f}; //  bias in uT


void initRobotMeuh()
{
 initSpiMasterMode(); // master SPI
 initI2C();           // I2C
 initRTC();
 initImus();
 initFusionImu();
 initStepperWeel();   // timer 3 & 4
 initBrushlessBlade();// timer 5
 initTaskScheduler(); // timer 0
}

int main(void)
{
 _delay_ms(500); // Wake up !
 initRobotMeuh(); // global initialisation
 sei(); // Enable interrupt
 lcdDispOffClear();
 for (uint8_t i = 0; i < 10; ++i)
  {
   lcdLedOff();
   _delay_ms(100);
   lcdLedOn();
   _delay_ms(100);
  }
 forceMenu(M_FIRST);
 _delay_ms(1000);

 forceMenu(M_STATUS);
 _delay_ms(1000);

 for (uint16_t i = 0; i < 10; ++i)
  {
   forceMenu(M_DATETIME);
   _delay_ms(500);
  }


 /*
 F-RAM     0x50
 PCF8563T  0x51
 ADXL345   0x53
 HMC5883L  0x1E
 ITG3205   0x68
 */
// test I2C address
 uint8_t addr = 0x00;
 do
  {
   if (!i2c_start(addr))
    {
     lcdClear();
     lcdPrintf(0, 0, PSTR("I2C -> %02X"), addr);
     _delay_ms(2000);
    }
   ++addr;
   _delay_ms(1);
  }
 while (addr <= 0x7F);


 computeFusionImu();


 BrushlessBladeCutAt(1000);
 _delay_ms(150);

 lcdPrintf(0, 0, PSTR("Lame%6i O%i"), BrushlessBlade.PWMValue, get_input(BladePwmOutPin));
 _delay_ms(5000);

 _delay_ms(100);
 lcdClear();
 enableStepperWheel();

 wheelAcceleration = 100;
 setWheelsSpeed(-32000,32000);
 do
  {
   _delay_ms(150);

   lcdPrintf(0, 0, PSTR("R%6i A%6i"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);

  }
 while(L_RequestSpeed != L_ActualSpeed);
 for (uint16_t i = 0; i < 200; ++i)
  {
   _delay_ms(100);
   lcdPrintf(0, 0, PSTR("R%6i A%6i"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);
  }
 lcdLedOff();
 _delay_ms(100);
 lcdLedOn();
 _delay_ms(100);

 do
  {
   _delay_ms(100);
  }
 while(1);


 return 0;
}

void setWheelsSpeed(int16_t L_Speed, int16_t R_Speed)
{
 L_RequestSpeed = L_Speed;
 R_RequestSpeed = R_Speed;
 SystemBools.whellSpeedOk = 0; // activate acceleration computation in fast task
}

// Fast task
void Task8mS()
{
 if (!SystemBools.whellSpeedOk) // if we need to compute wheels acceleration
  {
   SystemBools.whellSpeedOk = (computeStepperWheelSpeed())? 0 : 1;
  }

}

void Task32mS()
{
 BrushlessBladeUpdateRPM(); // todo call every 1 secondes ?

}
