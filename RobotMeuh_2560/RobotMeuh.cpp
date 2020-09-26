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

//SPI (LCD com.)
char SpiBuf[SPI_BUFFER_LENGHT] = {SPI_EOT};
volatile uint8_t SpiBufNum = 0;

//TIME
time_t rtcTime;

//IMU
FusionBias fusionBias;
FusionAhrs fusionAhrs;
float samplePeriod = 0.01f; // sample period in seconds
FusionVector3 gyroscopeSensitivity = {GYRO_RATE_XYZ, GYRO_RATE_XYZ, GYRO_RATE_XYZ};// sensitivity in degrees per second per lsb
FusionVector3 accelerometerSensitivity = {ACC_RATE_XYZ, ACC_RATE_XYZ, ACC_RATE_XYZ}; // Sensitivity in g per lsb
FusionVector3 hardIronBias = {0.0f, 0.0f, 0.0f}; //  bias in uT


void initRobotMeuh()
{
 i2c_init();
 initSpiMasterMode();
 rtcInit();
 initFusionImu();
 initStepperWeel();
}

int main(void)
{
 _delay_ms(500); // Wake up !
 initRobotMeuh();
 sei(); // Enable interrupt
 lcdDispOff();
 for (uint8_t i = 0; i < 10; ++i)
  {
   lcdLedOff();
   _delay_ms(100);
   lcdLedOn();
   _delay_ms(100);
  }
 lcdClear();
 _delay_ms(100);
 lcdPrintString(0, 2, PSTR("ROBOT MEUH !"));
 lcdDispOn();
 _delay_ms(1000);







 _delay_ms(100);
 lcdClear();
 enableStepperWheel();

 wheelAcceleration = 10;
 L_RequestSpeed = 10;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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


 wheelAcceleration = 10;
 L_RequestSpeed = 20;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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



   wheelAcceleration = 10;
 L_RequestSpeed = 50;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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

  wheelAcceleration = 10;
 L_RequestSpeed = 100;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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


  wheelAcceleration = 10;
 L_RequestSpeed = 900;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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


  wheelAcceleration = 100;
 L_RequestSpeed = 1500;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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


  wheelAcceleration = 100;
 L_RequestSpeed = 3000;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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


  wheelAcceleration = 100;
 L_RequestSpeed = 8000;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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

  wheelAcceleration = 150;
 L_RequestSpeed = 8000;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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

  wheelAcceleration = 200;
 L_RequestSpeed = 15000;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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


  wheelAcceleration = 250;
 L_RequestSpeed = 32000;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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

  wheelAcceleration = 250;
 L_RequestSpeed = -32000;
 do
  {
   _delay_ms(150);computeStepperWheelSpeed();
   lcdPrintf(0, 0, PSTR("R%6i A%6u"), L_RequestSpeed, L_ActualSpeed);
   lcdPrintf(1, 0, PSTR("P%3u St %6u"), (L_Prescaler & 3), L_StepCourse);computeStepperWheelSpeed();
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

