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

#ifndef PACK
 #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
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
#include "rtc.h"
#include "spi.h"
#include "i2c.h"
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

// Debug
#define ERR(x) {lcdPrintString(0, 0, PSTR(x));_delay_ms(1000);}

// PID
#define PID_SCALING_FACTOR  1024
#define Kp_Default          (1.000 * PID_SCALING_FACTOR)
#define Ki_Default          (0.200 * PID_SCALING_FACTOR)
#define Kd_Default          (0.050 * PID_SCALING_FACTOR)

/*
 I2C ADDRESS
F-RAM     0x50 // F-RAM (option)
PCF8563T  0x51 // RTC
ADXL345   0x53 // Acc
QMC5883L  0x0D // Mag
ITG3205   0x68 // Gyro
*/

// Robot values saved to eeprom
PACK(typedef struct
{
// Blade
 uint16_t  BladeSpeed = 3000; // T/Minute
 int16_t  Blade_P_Factor = Kp_Default;
 int16_t  Blade_I_Factor = Ki_Default;
 int16_t  Blade_D_Factor = Kd_Default;
// Motion
 uint8_t  MotionSpeed = 20; // M/Minute
 int16_t  Dir_P_Factor = Kp_Default;
 int16_t  Dir_I_Factor = Ki_Default;
 int16_t  Dir_D_Factor = Kd_Default;

}) RobotMeuh_t;


PACK(typedef struct
{
 uint8_t whellSpeedOk:1; // no acceleration needed
 uint8_t unused:7;
}) SystemBools_t;

PACK(typedef struct
{
// in degres * 10
 int16_t roll:12;
 int16_t pitch:12;
 int16_t yaw:12;
 int16_t heading:12;
}) ImuValues_t;

//ROBOTMEUH
extern RobotMeuh_t RobotMeuh;
extern Status_t RobotStatus;
extern DataLcdToMain_t lcdReport;
extern SystemBools_t SystemBools;
extern ImuValues_t ImuValues;

// Spi data
extern char SpiBuf[SPI_BUFFER_LENGHT];
extern volatile uint8_t SpiBufNum;

//TIME
extern time_t rtcTime;
extern uint8_t counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))

//FUNCTIONS
void Task1S();
void Task32mS();
void Task8mS();

#endif // __ROBOTMEUH_H
