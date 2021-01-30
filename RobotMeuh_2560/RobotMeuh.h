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


#ifndef __ROBOTMEUH_H_INCLUDED
#define __ROBOTMEUH_H_INCLUDED

// integer short name
#define u8 uint8_t
#define s8 int8_t
#define u16 uint16_t
#define s16 int16_t
#define u32 uint32_t
#define s32 int32_t
#define u64 uint64_t
#define s64 int64_t

#ifndef PACK
 #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>

#include "pin_helper.h"
#include "pin.h"
#include "utils.h"
#include "Protocol.h"
#include "eeprom.h"
#include "rtc.h"
//#include "spi.h"
#include "i2c.h"
#include "AlarmClock.h"
#include "SerialCli.h"
#include "SerialLcd.h"
#include "Imu.h"
#include "AnalogSensor.h"
#include "lib/Fusion.h"
#include "FusionImu.h"
#include "StepperWheel.h"
#include "BrushlessBlade.h"
#include "Motion.h"
#include "LcdFunc.h"
#include "TaskScheduler.h"
#include "MenuManagement.h"
#include "MenuGeneral.h"

// robot description
#define WHELLBASE           60      // cM
#define WHEELDIAMETER       27.0f   // cM

#define WEELPERIMETER       WHEELDIAMETER * M_PI

#define MICROSTEP           32.0f   // DRV8825 uStepping used
#define STEPPERREV          200.0f
#define GEARREDUCTION       5.0f    // 11 / 55 gear ration (To test ...)

#define MAXSTEPPERSPEED     32000  // 5rev./seconde
#define MAXROBOTSPEED       28000

#define ROBOTSPEEDFACTOR    (WEELPERIMETER / (GEARREDUCTION * STEPPERREV * MICROSTEP))
#define MAXWHEELSPEED       (WEELPERIMETER / (GEARREDUCTION * STEPPERREV * MICROSTEP)) * MAXSTEPPERSPEED // 84.8 cM/Sec
#define MINWHEELSPEED       (WEELPERIMETER / (GEARREDUCTION * STEPPERREV * MICROSTEP)) * 1               // 0.00265 cM/Sec

// Debug
#define ERR(x) {lcdPrintString_P(0, 0, PSTR(x));sendLcdDispOn();_delay_ms(1000);}

// PID
#define PID_SCALING_FACTOR  1024
#define PID_K(x)            (x * PID_SCALING_FACTOR)
#define Kp_Default          PID_K(0.500)
#define Ki_Default          PID_K(0.080)
#define Kd_Default          PID_K(0.050)

/*
 I2C ADDRESS
F-RAM     0x50 // F-RAM (option)
PCF8563T  0x51 // RTC
ADXL345   0x53 // Acc
QMC5883L  0x0D // Mag
ITG3205   0x68 // Gyro
*/

PACK(typedef struct
{
 u8 lcdHeartBeatMem:1; // lcd heartbeat memory
 u8 lcdISOk:1; // check lcd heartbeat : 0 -> fault
 u8 toggle500mS:1; // 0.5S toggle
 u8 unused:5;
}) SystemBools_t;

PACK(typedef struct
{
// in degres * 10
 s16 roll:12;
 s16 pitch:12;
 s16 yaw:12;
 s16 heading:12;
}) ImuValues_t;

PACK(typedef struct
{
 u8 minute:6;
 u8 hour:5;
 u8 dayMask:7;
 u8 duration:6; // X 5 minutes
}) Alarm_t;
#define ALMDURATIONLENGHT  5

// Robot values saved to eeprom
PACK(typedef struct
{
// Blade
 u16  BladeSpeed:13; // T/Minute 5000 max
 u16  unused:3;
 s16  Blade_P_Factor;
 s16  Blade_I_Factor;
 s16  Blade_D_Factor;
// SteppersWheels
 u16  WheelsSpeed:9; // M/Minute 50.0 max
 u16  WheelsRotationRate:7; // 0 - 100 %
 s16  SW_P_Factor;
 s16  SW_I_Factor;
 s16  SW_D_Factor;
// Motion angle
 s16  Dir_P_Factor;
 s16  Dir_I_Factor;
 s16  Dir_D_Factor;
// Alarms
 Alarm_t FirstAlarm;
 Alarm_t SecondAlarm;
}) RobotMeuh_t;


//ROBOTMEUH
extern RobotMeuh_t RobotMeuh;
extern Status_t RobotStatus;
extern DataLcdToMain_t lcdReport;
extern SystemBools_t SystemBools;
extern ImuValues_t ImuValues;


//TIME
extern time_t rtcTime;
extern u8 counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))

//FUNCTIONS
void StackPaint() __attribute__ ((naked)) __attribute__ ((section (".init1")));
void robotMeuhSetDefault();
void Task1S();
void Task32mS();
void Task8mS();

#endif // __ROBOTMEUH_H
