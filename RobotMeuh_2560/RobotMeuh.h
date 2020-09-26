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
#include "gy85.h"
#include "AnalogSensor.h"
#include "lib/Fusion.h"
#include "FusionImu.h"
#include "lib/simplePID.h"
#include "StepperWheel.h"
#include "LcdFunc.h"

//Debug
#define ERR(x)

//ROBOTMEUH
extern Status_t RobotStatus;
extern DataLcdToMain_t lcdReport;

// Spi data
extern char SpiBuf[SPI_BUFFER_LENGHT];
extern volatile uint8_t SpiBufNum;

//TIME
extern time_t rtcTime;

//IMU
extern FusionBias fusionBias;
extern FusionAhrs fusionAhrs;
extern float samplePeriod; // sample period in seconds
extern FusionVector3 gyroscopeSensitivity;// sensitivity in degrees per second per lsb
extern FusionVector3 accelerometerSensitivity; // Sensitivity in g per lsb
extern FusionVector3 hardIronBias; //  bias in uT

#endif // __ROBOTMEUH_H
