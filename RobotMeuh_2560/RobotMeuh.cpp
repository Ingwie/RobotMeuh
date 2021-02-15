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

//TIME
time_t rtcTime;
u8 counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))


void robotMeuhSetDefault()
{
 memset(&RobotMeuh, 0, sizeof(RobotMeuh_t)); // reset all

 RobotMeuh.Battery = 0;
 RobotMeuh.BatteryAlarm = 245; // 24.5 V
 RobotMeuh.BladeSpeed = 3000; // T/Minute 5000 max
//RobotMeuh.unused = 0;
 RobotMeuh.Blade_P_Factor = Kp_Default;
 RobotMeuh.Blade_I_Factor = Ki_Default;
 RobotMeuh.Blade_D_Factor = Kd_Default;
// SteppersWheels
 RobotMeuh.WheelsSpeed = 200; // M/Minute 50.0 max
 RobotMeuh.WheelsRotationSpeedRate = 25; // 0 - 100 %
 RobotMeuh.SW_P_Factor = Kp_Default;
 RobotMeuh.SW_I_Factor = Ki_Default;
 RobotMeuh.SW_D_Factor = Kd_Default;
// Motion angle
 RobotMeuh.Dir_P_Factor = Kp_Default;
 RobotMeuh.Dir_I_Factor = Ki_Default;
 RobotMeuh.Dir_D_Factor = Kd_Default;
}

void initRobotMeuh()
{
 initSerialCli();
 initSerialLcd();     // Lcd com.
//initSpiMasterMode(); // master SPI
 initI2C();           // I2C
//readEeprom();      // Todo Fram ?
 initRTC();
 initImus();
 initFusionImu();
 initStepperWeel();   // timer 3 & 4
 initBrushlessBlade();// timer 5
 adcInit();
//initTaskScheduler(); // timer 0 wait ... init later
}

int main(void)
{
 _delay_ms(500); // Wake up !
 eepromReadAll(); // load RobotMeuh settings
 initRobotMeuh(); // global initialisation
 readBatteryVoltage();
 sei(); // Enable interrupt
 lcdLoadCgram(customChars, 8); // load custom characteres in CGram
 sendLcdDispOffClear();
 for (u8 i = 0; i < 10; ++i)
  {
   sendLcdLedOff();
   _delay_ms(90);
   sendLcdLedOn();
   _delay_ms(90);
  }
 sendLcdDispOn();
 forceMenu(M_FIRST); // welcome !
 _delay_ms(1500);
 forceMenu(M_STATUS);
 forceBrain(B_WAKEUP);
// start tasks
 initTaskScheduler(); // timer 0


 u8 onetime = 0;
 do
  {
   _delay_ms(300);
   sendLcdDispOffClear();

// TEST //
#define TESTMENU

   if (!onetime)
    {
     BrushlessBladeCutAt(9000);
     //ADCSRA |= _BV(ADSC); // Start the AD conversion
     onetime = 1;
    }

#if defined (TESTMENU)
   // Show menus
   do
    {
     for (u16 i = 0; i < 50; ++i)
      {
       SerialCliPrint("totototototo");
       SerialCliSend();
       if (!SystemBools.lcdISOk) ERR("PB ecran"); // todo remove
       if (0)//(isTimeToWork())
        {
         if (SystemBools.toggle500mS)
          {
           sendLcdLedOn();
          }
         else
          {
           sendLcdLedOff();
          }
        }
       menuCompute();

       //forceMenu((menuArray)onetime);
       _delay_ms(200);
      }
     if (++onetime == M_MENUNUMBER) onetime = M_FIRST;
    }
   while (1);
#endif
#if defined (EULER)
   forceMenu(M_IMUFUSION);
#endif
#if defined (GYRO)
   forceMenu(M_IMUGYRO);
#endif
#if defined (ACC)
   forceMenu(M_IMUACC);
#endif
#if defined (MAG)
   forceMenu(M_IMUMAG);
#endif
#if defined (SW)
   forceMenu(M_WHEELSPULSES);
#endif
   lcdDispOn();
  }
 while(1);


 return 0;
}

// Fast task
void Task8mS()
{
 computeStepperWheelSpeed();
}

// Mid task
void Task32mS()
{
 computeFusionImu(); // Compute imus (approx 150uS)
//forceStepperWheelPulses(32000, decimeterPerMinuteToPulses(5000));
 //Motion_FollowAngle(decimeterPerMinuteToPulses(RobotMeuh.WheelsSpeed), 900);
//Motion_Turn(100, 0);
 brainCompute();
 checkSerialLcdRXBuf();
}

// Slow task
void Task1S() // ISR mode
{
 if (BrushlessBlade.IsCutting)
  {
   BrushlessBladeUpdate(); // this function also reset ISR mode
  }
// update temperature (from gyro)
 readGyroTemp();
// update battery voltage
 readBatteryVoltage();
}

/////////////////// INCLUDE ALL CPP AND C FILES HERE ////////////////////////

#include "utils.cpp"
#include "eeprom.cpp"
#include "rtc.cpp"
//#include "spi.cpp"
#include "i2c.cpp"
#include "AlarmClock.cpp"
#include "SerialCli.cpp"
#include "SerialLcd.cpp"
#include "Imu.cpp"
#include "AnalogSensor.cpp"
#include "PowerManagement.cpp"
#include "lib/FusionAhrs.c"
#include "lib/FusionBias.c"
#include "lib/FusionCompass.c"
#include "FusionImu.cpp"
#include "StepperWheel.cpp"
#include "BrushlessBlade.cpp"
#include "Motion.cpp"
#include "LcdFunc.cpp"
#include "TaskScheduler.cpp"
#include "MenuManagement.cpp"
#include "MenuGeneral.cpp"
#include "BrainModes.cpp"
