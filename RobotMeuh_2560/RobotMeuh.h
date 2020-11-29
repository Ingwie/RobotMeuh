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
#include "lib/simplePID.h"
#include "StepperWheel.h"
#include "BrushlessBlade.h"
#include "LcdFunc.h"
#include "TaskScheduler.h"
#include "MenuManagement.h"
#include "MenuGeneral.h"

//Debug
#define ERR(x) {lcdPrintString(0, 0, PSTR(x));_delay_ms(1000);}

PACK(typedef struct
{
 uint8_t whellSpeedOk:1; // no acceleration needed
 uint8_t unused:7;
}) SystemBools_t;


//ROBOTMEUH
extern Status_t RobotStatus;
extern DataLcdToMain_t lcdReport;
extern SystemBools_t SystemBools;

// Spi data
extern char SpiBuf[SPI_BUFFER_LENGHT];
extern volatile uint8_t SpiBufNum;

//TIME
extern time_t rtcTime;
extern uint8_t counter8mS; // Updated in TaskScheduler (ISR(TIMER0_COMPA_vect))

//FUNCTIONS
void Task32mS();
void Task8mS();
void setWheelsSpeed(int16_t L_Speed, int16_t R_Speed);

#endif // __ROBOTMEUH_H
