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

#ifndef MENUGENERAL_H_INCLUDED
#define MENUGENERAL_H_INCLUDED

#include "RobotMeuh.h"

const void menuFirst(); // First at boot
const void menuStatus(); // show status
const void menuLcdKeys(); // show lcd keys
const void menuDateTime(); // show date time
const void menuFreeSram(); // show free sram
const void menuImuFusion(); // Euler datas
const void menuImuGyro(); // Gyroscope datas
const void menuImuMag(); // Magnetometer datas
const void menuBladeSpeed(); // Blade speed setting
const void menuPidBlade(); // PID blade settings
const void menuWheelsSpeed(); // Wheels speed setting
const void menuPidDirection(); // PID direction settings
const void menuWheelsRotationRate(); // Wheels rotation rate setting
const void menuTemperature(); // show gyro temperature
const void menuResetEeprom(); // restore default value
const void menuSetFirstAlarm(); // Set firt alarm
const void menuSetSecondAlarm(); // Set firt alarm
const void menuTestSteppers(); // Test wheels motors and controlers
const void menuTestBrushless(); // Test blade motor and controler


const char EMPTYCHAR = '_';
const char HOUR_MASK[] PROGMEM = "%02u:%02u:%02u";
const char HOURMINUTE_MASK[] PROGMEM = "%02u:%02u";
const char IMU_YZ_MASK[] PROGMEM = "Y % 5i Z % 5i";
const char PID_ID_MASK[] PROGMEM = "I:%01u.%03u  D:%01u.%03u";
const char WEEK_ALM[] PROGMEM = "-DLMMJVS-";

#endif
