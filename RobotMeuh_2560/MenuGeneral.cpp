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

#include "MenuGeneral.h"

const void menuFirst() // First at boot
{
 lcdPrintString_P(0, 2, PSTR("ROBOT MEUH !"));
 lcdPrintString_P(1, 4, PSTR("\4 V001 \4"));
}

const void menuStatus() // show status
{
 if (!SystemBools.toggle500mS)
  lcdPrintString_P(0, 0, PSTR("***          ***"));
 else
  lcdPrintString_P(0, 0, PSTR("+++          +++"));
 div_t L = div(pulsesToDecimeterPerMinute(L_ActualSpeed), 10);
 div_t R = div(pulsesToDecimeterPerMinute(R_ActualSpeed), 10);
//lcdPrintf(1, 0, PSTR("%06i    %06i"), L_ActualSpeed, R_ActualSpeed);
 lcdPrintf(1, 0, PSTR("%02i.%01i        %02i.%01i"), L.quot, L.rem, R.quot, R.rem);
 menuNavigation(PMT(M_LCDKEYS, M_TESTBRUSHLESS, M_STATUS, M_STATUS));
}

const void menuLcdKeys() // show lcd keys
{
 lcdPrintString_P(0, 0, PSTR("Test clavier(+-)"));
 if (lcdReport.KeyHome) lcdPrintString_P(1, 0, PSTR("\7"));
 if (lcdReport.KeyEnter) lcdPrintString_P(1, 1, PSTR("\1"));
 if (lcdReport.KeyPlay) lcdPrintString_P(1, 2, PSTR("\b"));
 if (lcdReport.KeyPlus) lcdPrintString_P(1, 3, PSTR("+"));
 if (lcdReport.KeyMinus) lcdPrintString_P(1, 4, PSTR("-"));
//if (lcdReport.KeyPlus && lcdReport.KeyMinus) // +- allow exit
 {
  menuVar.editMode = 0; // reset
  menuNavigation(PMT(M_DATETIME, M_STATUS, M_LCDKEYS, M_LCDKEYS));
 }
}

const void menuDateTime() // show date time
{
 struct tm * utm = localtime(&rtcTime);

 lcdPrintf(0, 3, PSTR("%02u/%02u/%u"), utm->tm_mday, utm->tm_mon + 1, utm->tm_year + 1900);
 lcdPrintf(1, 4, HOUR_MASK, utm->tm_hour, utm->tm_min, utm->tm_sec);

 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 5; // set max field
   _delay_ms(1); // time to send and free some buffers
   switch (menuVar.field)
    {
    case 1:
     utm->tm_mday = setMenuValue(utm->tm_mday, 31, 0, 1);
     lcdLocate(0,4);
     break;
    case 2:
     utm->tm_mon = setMenuValue(utm->tm_mon, 11, 0, 1);
     lcdLocate(0,7);
     break;
    case 3:
     utm->tm_year = setMenuValue(utm->tm_year, 199, 121, 1);
     lcdLocate(0,12);
     break;
    case 4:
     utm->tm_hour = setMenuValue(utm->tm_hour, 23, 0, 1);
     lcdLocate(1,5);
     break;
    case 5:
     utm->tm_min = setMenuValue(utm->tm_min, 59, 0, 1);
     lcdLocate(1,8);
     break;
    }

   rtcTime = mk_gmtime(utm);

   if (menuVar.wasEdited && !menuVar.editMode) rtcSetTime(utm); // update RTC chip
  }
 menuNavigation(PMT(M_FREESRAM, M_LCDKEYS, M_DATETIME, M_DATETIME));
}

const void menuFreeSram() // show free sram
{
 lcdPrintString_P(0, 3, PSTR("SRAM libre:"));
 u16 fs = freeSram();
 lcdPrintf(1, 3, PSTR("%04u Octets"), fs);
 menuNavigation(PMT(M_IMUFUSION, M_DATETIME, M_FREESRAM, M_FREESRAM));
}

const void menuImuFusion() // Euler datas
{
 lcdPrintf(0,0,PSTR("R% 4i.%01i T% 4i.%01i"), ImuValues.roll/10, abs(ImuValues.roll)%10, ImuValues.pitch/10, abs(ImuValues.pitch)%10);
 lcdPrintf(1,0,PSTR("L% 4i.%01i N% 4i.%01i"), ImuValues.yaw/10, abs(ImuValues.yaw)%10, ImuValues.heading/10, abs(ImuValues.heading)%10);
 menuNavigation(PMT(M_IMUGYRO, M_FREESRAM, M_IMUFUSION, M_IMUFUSION));
}

const void menuImuGyro() // Gyroscope raw datas
{
 lcdPrintf(0,0,PSTR("Gyro : X % 5i"), imuGyro.x);
 lcdPrintf(1,0, IMU_YZ_MASK, imuGyro.y, imuGyro.z);
 menuNavigation(PMT(M_IMUACC, M_IMUFUSION, M_IMUGYRO, M_IMUGYRO));
}

const void menuImuAcc() // Accelerometer raw datas
{
 lcdPrintf(0,0,PSTR("Accel: X % 5i"), imuAcc.x);
 lcdPrintf(1,0, IMU_YZ_MASK, imuAcc.y, imuAcc.z);
 menuNavigation(PMT(M_IMUMAG, M_IMUGYRO, M_IMUMAG, M_IMUACC));
}

const void menuImuMag() // Accelerometer raw datas
{
 lcdPrintf(0,0,PSTR("Magne: X % 5i"), imuMag.x);
 lcdPrintf(1,0, IMU_YZ_MASK, imuMag.y, imuMag.z);
 menuNavigation(PMT(M_WHEELSPULSES, M_IMUACC, M_IMUMAG, M_IMUMAG));
}

const void menuWheelsPulses() // Wheels pulses datas
{
 lcdPrintf(0, 0, PSTR("L%6i A%6i"), L_RequestSpeed, L_ActualSpeed);
 lcdPrintf(1, 0, PSTR("R%6i A%6i"), R_RequestSpeed, R_ActualSpeed);
//lcdPrintf(1, 0, PSTR("V%4i St%8u"), wheelActualSpeed, L_StepCourse);
 menuNavigation(PMT(M_BLADESPEED, M_IMUMAG, M_WHEELSPULSES, M_WHEELSPULSES));
}

const void menuBladeSpeed() // Blade speed setting
{
 lcdPrintString_P(0, 2, PSTR("Vitesse lame"));
 lcdPrintf(1, 4, PSTR("%4u Tr/m"), RobotMeuh.BladeSpeed);
 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 1; // set max field
   _delay_ms(1); // time to send and free some buffers
   RobotMeuh.BladeSpeed = setMenuValue(RobotMeuh.BladeSpeed, 5000, 0, 50);
   lcdLocate(1,7);
  }
 menuNavigation(PMT(M_PIDBLADE, M_WHEELSPULSES, M_BLADESPEED, M_BLADESPEED));
}


const void menuPidBlade() // PID blade settings
{
 div_t P = div(RobotMeuh.Blade_P_Factor, PID_SCALING_FACTOR);
 div_t I = div(RobotMeuh.Blade_I_Factor, PID_SCALING_FACTOR);
 div_t D = div(RobotMeuh.Blade_D_Factor, PID_SCALING_FACTOR);
 lcdPrintf(0, 0, PSTR("PID Lame: %01u.%03u"), P.quot, P.rem);
 lcdPrintf(1, 0, PID_ID_MASK, I.quot, I.rem, D.quot, D.rem);

 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 3; // set max field
   _delay_ms(1); // time to send and free some buffers
   switch (menuVar.field)
    {
    case 1:
     RobotMeuh.Blade_P_Factor = setMenuValue(RobotMeuh.Blade_P_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(0,14);
     break;
    case 2:
     RobotMeuh.Blade_I_Factor = setMenuValue(RobotMeuh.Blade_I_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(1,6);
     break;
    case 3:
     RobotMeuh.Blade_D_Factor = setMenuValue(RobotMeuh.Blade_D_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(1,15);
     break;
    }
  }
 if (menuVar.wasEdited && !menuVar.editMode) initBladePid(); // init on changes
 menuNavigation(PMT(M_WHEELSSPEED, M_BLADESPEED, M_PIDBLADE, M_PIDBLADE));
}

const void menuWheelsSpeed() // Wheels speed setting
{
 div_t W = div(RobotMeuh.WheelsSpeed, 10);
 lcdPrintString_P(0, 0, PSTR("Vitesse lineaire"));
 lcdPrintf(1, 4, PSTR("%2u.%01u M/m"), W.quot, W.rem);
 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 1; // set max field
   _delay_ms(1); // time to send and free some buffers
   RobotMeuh.WheelsSpeed = setMenuValue(RobotMeuh.WheelsSpeed, 500, 0, 1);
   lcdLocate(1,7);
  }
 if (menuVar.wasEdited && !menuVar.editMode) eepromWritedAll(); // write to eeprom
 menuNavigation(PMT(M_WHEELSROTRATE, M_PIDBLADE, M_WHEELSSPEED, M_WHEELSSPEED));
}

const void menuWheelsRotationRate() // Wheels rotation rate setting
{
 lcdPrintString_P(0, 0, PSTR("Taux de rotation"));
 lcdPrintf(1, 6, PSTR("%03u %%"), RobotMeuh.WheelsRotationRate);
 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 1; // set max field
   _delay_ms(1); // time to send and free some buffers
   RobotMeuh.WheelsRotationRate = setMenuValue(RobotMeuh.WheelsRotationRate, 100, 0, 1);
   lcdLocate(1,8);
  }
 menuNavigation(PMT(M_PIDWHEELS, M_WHEELSSPEED, M_WHEELSROTRATE, M_WHEELSROTRATE));
}

const void menuPidWheels() // PID wheels settings
{
 div_t P = div(RobotMeuh.SW_P_Factor, PID_SCALING_FACTOR);
 div_t I = div(RobotMeuh.SW_I_Factor, PID_SCALING_FACTOR);
 div_t D = div(RobotMeuh.SW_D_Factor, PID_SCALING_FACTOR);
 lcdPrintf(0, 0, PSTR("PID roues: %01u.%03u"), P.quot, P.rem);
 lcdPrintf(1, 0, PID_ID_MASK, I.quot, I.rem, D.quot, D.rem);

 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 3; // set max field
   _delay_ms(1); // time to send and free some buffers
   switch (menuVar.field)
    {
    case 1:
     RobotMeuh.SW_P_Factor = setMenuValue(RobotMeuh.SW_P_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(0,15);
     break;
    case 2:
     RobotMeuh.SW_I_Factor = setMenuValue(RobotMeuh.SW_I_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(1,6);
     break;
    case 3:
     RobotMeuh.SW_D_Factor = setMenuValue(RobotMeuh.SW_D_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(1,15);
     break;
    }
  }
 if (menuVar.wasEdited && !menuVar.editMode) initStepperPid(); // init on changes
 menuNavigation(PMT(M_PIDDIR, M_WHEELSROTRATE, M_PIDWHEELS, M_PIDWHEELS));
}

const void menuPidDirection() // PID direction settings
{
 div_t P = div(RobotMeuh.Dir_P_Factor, PID_SCALING_FACTOR);
 div_t I = div(RobotMeuh.Dir_I_Factor, PID_SCALING_FACTOR);
 div_t D = div(RobotMeuh.Dir_D_Factor, PID_SCALING_FACTOR);
 lcdPrintf(0, 0, PSTR("PID cap:  %01u.%03u"), P.quot, P.rem);
 lcdPrintf(1, 0, PID_ID_MASK, I.quot, I.rem, D.quot, D.rem);

 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 3; // set max field
   _delay_ms(1); // time to send and free some buffers
   switch (menuVar.field)
    {
    case 1:
     RobotMeuh.Dir_P_Factor = setMenuValue(RobotMeuh.Dir_P_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(0,14);
     break;
    case 2:
     RobotMeuh.Dir_I_Factor = setMenuValue(RobotMeuh.Dir_I_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(1,6);
     break;
    case 3:
     RobotMeuh.Dir_D_Factor = setMenuValue(RobotMeuh.Dir_D_Factor, 9*PID_SCALING_FACTOR, 0, 1);
     lcdLocate(1,15);
     break;
    }
  }
 if (menuVar.wasEdited && !menuVar.editMode) initDirPid(); // init on changes
 menuNavigation(PMT(M_TEMPERATURE, M_PIDWHEELS, M_PIDDIR, M_PIDDIR));
}

const void menuTemperature() // show gyro temperature
{
 lcdPrintString_P(0, 2, PSTR("Temperature:"));
 div_t T = div(gyroTemp, 10);
 lcdPrintf(1, 5, PSTR("%02u.%01uC"), T.quot, T.rem);
 menuNavigation(PMT(M_RESETEEPROM, M_PIDDIR, M_TEMPERATURE, M_TEMPERATURE));
}

const void menuResetEeprom() // restore default value
{
 lcdPrintString_P(0, 2, PSTR("Reset eeprom"));
 if (menuVar.editMode)
  {
   robotMeuhSetDefault(); // reset to default values
   eepromWritedAll(); // write to eeprom
   lcdPrintString_P(1, 5, PSTR("Fait"));

  }
 menuNavigation(PMT(M_SETFIRSTALARM, M_TEMPERATURE, M_RESETEEPROM, M_RESETEEPROM));
}

const void menuSetFirstAlarm() // Set first alarm
{
 lcdPrintf(0, 0, PSTR("\2\5   %03u minutes"), RobotMeuh.FirstAlarm.duration * 5);
 lcdPrintf(1, 1, HOURMINUTE_MASK, RobotMeuh.FirstAlarm.hour, RobotMeuh.FirstAlarm.minute);

 char weekStr[10];
 memcpy_P(weekStr, &WEEK_ALM, sizeof(WEEK_ALM));
 weekStr[sizeof(weekStr)-1] = 0; // end of string

 struct tm * utm = localtime(&rtcTime);

 for (u8 i=0; i<7; ++i)
  {
   if (i == utm->tm_wday) weekStr[1 + i] += 32; // make lowercase if day is now
   if ((RobotMeuh.FirstAlarm.dayMask & _BV(i)) == 0)
    {
     weekStr[1 + i] = EMPTYCHAR;
    }
  }
 lcdPrintString(1, 7, weekStr);

 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 10; // set max field
   _delay_ms(1); // time to send and free some buffers
   switch (menuVar.field)
    {
    case 1:
     RobotMeuh.FirstAlarm.duration = setMenuValue(RobotMeuh.FirstAlarm.duration, 36, 0, 1);
     lcdLocate(0,7);
     break;
    case 2:
     RobotMeuh.FirstAlarm.hour = setMenuValue(RobotMeuh.FirstAlarm.hour, 23, 0, 1);
     lcdLocate(1,2);
     break;
    case 3:
     RobotMeuh.FirstAlarm.minute = setMenuValue(RobotMeuh.FirstAlarm.minute, 59, 0, 1);
     lcdLocate(1,5);
     break;
    default:
     u8 pos = menuVar.field - 4; // save position
     u8 val = (RobotMeuh.FirstAlarm.dayMask & _BV(pos))? 1 : 0;
     val = setMenuValue(val, 1, 0, 1);
     u8 mask = _BV(pos);
     if (pos == (menuVar.field - 4)) // if field is not changed
      {
       if (val) RobotMeuh.FirstAlarm.dayMask |= mask;
       else RobotMeuh.FirstAlarm.dayMask &= ~mask;
      }
     lcdLocate(1, 4 + menuVar.field); // blink correct field
     break;
    }
  }
 if (menuVar.wasEdited && !menuVar.editMode)
  {
   eepromWritedAll(); // write to eeprom
   sheduleNextAlarm(); // write to RTC
  }

 menuNavigation(PMT(M_SETSECONDTALARM, M_RESETEEPROM, M_SETFIRSTALARM, M_SETFIRSTALARM));
}

const void menuSetSecondAlarm() // Set second alarm
{
 lcdPrintf(0, 0, PSTR("\2\5\5  %03u minutes"), RobotMeuh.SecondAlarm.duration * 5);
 lcdPrintf(1, 1, HOURMINUTE_MASK, RobotMeuh.SecondAlarm.hour, RobotMeuh.SecondAlarm.minute);

 char weekStr[10];
 memcpy_P(weekStr, &WEEK_ALM, sizeof(WEEK_ALM));
 weekStr[sizeof(weekStr)-1] = 0; // end of string

 struct tm * utm = localtime(&rtcTime);

 for (u8 i=0; i<7; ++i)
  {
   if (i == utm->tm_wday) weekStr[1 + i] += 32; // make lowercase if day is now
   if ((RobotMeuh.SecondAlarm.dayMask & _BV(i)) == 0)
    {
     weekStr[1 + i] = EMPTYCHAR;
    }
  }
 lcdPrintString(1, 7, weekStr);

 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 10; // set max field
   _delay_ms(1); // time to send and free some buffers
   switch (menuVar.field)
    {
    case 1:
     RobotMeuh.SecondAlarm.duration = setMenuValue(RobotMeuh.SecondAlarm.duration, 36, 0, 1);
     lcdLocate(0,7);
     break;
    case 2:
     RobotMeuh.SecondAlarm.hour = setMenuValue(RobotMeuh.SecondAlarm.hour, 23, 0, 1);
     lcdLocate(1,2);
     break;
    case 3:
     RobotMeuh.SecondAlarm.minute = setMenuValue(RobotMeuh.SecondAlarm.minute, 59, 0, 1);
     lcdLocate(1,5);
     break;
    default:
     u8 pos = menuVar.field - 4; // save position
     u8 val = (RobotMeuh.SecondAlarm.dayMask & _BV(pos))? 1 : 0;
     val = setMenuValue(val, 1, 0, 1);
     u8 mask = _BV(pos);
     if (pos == (menuVar.field - 4)) // if field is not changed
      {
       if (val) RobotMeuh.SecondAlarm.dayMask |= mask;
       else RobotMeuh.SecondAlarm.dayMask &= ~mask;
      }
     lcdLocate(1, 4 + menuVar.field); // blink correct field
     break;
    }
  }
 if (menuVar.wasEdited && !menuVar.editMode)
  {
   eepromWritedAll(); // write to eeprom
   sheduleNextAlarm(); // write to RTC
  }

 menuNavigation(PMT(M_TESTSTEPPERS, M_SETFIRSTALARM, M_SETSECONDTALARM, M_SETSECONDTALARM));
}

const void menuTestSteppers() // Test wheels motors and controlers
{
 lcdPrintString_P(0, 0, PSTR("Test Propulsion"));
 div_t L = div(pulsesToDecimeterPerMinute(L_ActualSpeed), 10);
 div_t R = div(pulsesToDecimeterPerMinute(R_ActualSpeed), 10);
 s16 testSpeed = decimeterPerMinuteToPulses(menuVar.value * 6);
 forceStepperWheelPulses(testSpeed, testSpeed);
 div_t T = div(pulsesToDecimeterPerMinute(testSpeed), 10);
 lcdPrintf(1, 0, PSTR("%02i.%01i  %02i.%01i  %02i.%01i"), L.quot, L.rem, T.quot, T.rem, R.quot, R.rem);
 if (menuVar.editMode)
  {
   if (!get_output(L_WheelEnablePin)) enableStepperWheel(); // enable steppers controler if they are "OFF"
   SerialLcdSend(); // send buffer
   menuVar.maxField = 1; // set max field
   _delay_ms(1); // time to send and free some buffers
   menuVar.value = setMenuValue(menuVar.value, 63, 0, 1);
   lcdLocate(1,9);
  }
 else
  {
   menuVar.value = 0; // Reset value
   if (menuVar.wasEdited)disableStepperWheel();
  }
 menuNavigation(PMT(M_TESTBRUSHLESS, M_SETSECONDTALARM, M_TESTSTEPPERS, M_TESTSTEPPERS));
}

const void menuTestBrushless() // Test blade motor and controler
{
 lcdPrintString_P(0, 1, PSTR("Test Mot. lame"));
 s16 testSpeed = menuVar.value * 200;
 BrushlessBladeCutAt(testSpeed);
 lcdPrintf(1, 6, PSTR("%03u %%"), testSpeed / 100);
 if (menuVar.editMode)
  {
   SerialLcdSend(); // send buffer
   menuVar.maxField = 1; // set max field
   _delay_ms(1); // time to send and free some buffers
   menuVar.value = setMenuValue(menuVar.value, 50, 0, 1);
   lcdLocate(1,8);
  }
 else if (menuVar.wasEdited)
  {
   menuVar.value = 0; // Reset value
   BrushlessBladeStop();
  }
 menuNavigation(PMT(M_STATUS, M_TESTSTEPPERS, M_TESTBRUSHLESS, M_TESTBRUSHLESS));
}
