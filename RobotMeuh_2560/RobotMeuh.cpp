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
RobotMeuh_t RobotMeuh;
Status_t RobotStatus = {0};
DataLcdToMain_t lcdReport = {0};
SystemBools_t SystemBools = {0};
ImuValues_t ImuValues = {0};

//SPI (LCD com.)
char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
volatile uint8_t SpiBufNum = 0;

//TIME
time_t rtcTime;
uint8_t counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))


void initRobotMeuh()
{
 initSpiMasterMode(); // master SPI
 initI2C();           // I2C
 //readEeprom();      // Todo Fram ?
 initRTC();
 initImus();
 initFusionImu();
 initStepperWeel();   // timer 3 & 4
 initDirPid();
 initBrushlessBlade();// timer 5
 initBladePid();
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
 /*
  forceMenu(M_STATUS);
  _delay_ms(1000);

  for (uint16_t i = 0; i < 2; ++i)
   {
    forceMenu(M_DATETIME);
    _delay_ms(500);
   }
 */




 uint8_t onetime = 0;
 do
  {
   _delay_ms(300);
   lcdDispOffClear();

// TEST //
#define SW

   if (!onetime)
    {
     BrushlessBladeCutAt(9000);
     wheelAcceleration = 50;
     enableStepperWheel();
     onetime = 1;
    }

#if defined (EULER)
   // Print Euler angles and heading
   lcdPrintf(0,0,PSTR("R:%04i P:%04i"), ImuValues.roll, ImuValues.pitch);
   lcdPrintf(1,0,PSTR("Y:%04i H:%04i"), ImuValues.yaw, ImuValues.heading);
#endif
#if defined (GYRO)
   lcdPrintf(0,0,PSTR("X:%05i Y:%05i"), imuGyro.x, imuGyro.y);
   lcdPrintf(1,0,PSTR("Z:%05i"), imuGyro.z);
#endif
#if defined (ACC)
   lcdPrintf(0,0,PSTR("X:%05i Y:%05i"), imuAcc.x, imuAcc.y);
   lcdPrintf(1,0,PSTR("Z:%05i"), imuAcc.z);
#endif
#if defined (MAG)
   lcdPrintf(0,0,PSTR("X:%05i Y:%05i"), imuMag.x, imuMag.y);
   lcdPrintf(1,0,PSTR("Z:%05i"), imuMag.z);
#endif
#if defined (SW)
   lcdPrintf(0, 0, PSTR("L%6i A%6i"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("R%6i A%6i"), R_RequestSpeed, R_ActualSpeed);
   //lcdPrintf(1, 0, PSTR("V%4i St%8u"), wheelActualSpeed, L_StepCourse);
#endif
   lcdDispOn();
  }
 while(1);


 return 0;
}

// Fast task
void Task8mS()
{
 if (!SystemBools.whellSpeedOk) // if we need to compute wheels acceleration
  {
   SystemBools.whellSpeedOk = (computeStepperWheelSpeed())? 0 : 1;
  }

}

// Mid task
void Task32mS()
{
 computeFusionImu(); // Compute imus (approx 150uS)
 followCourse(20000, 900);
}

// Slow task
void Task1S() // ISR mode
{
 BrushlessBladeReadRPM();
 sei(); // reset ISR mode
}
