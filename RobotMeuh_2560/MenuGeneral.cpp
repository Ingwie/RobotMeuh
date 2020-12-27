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

void menuFirst() // First at boot
{
 lcdPrintString(0, 2, PSTR("ROBOT MEUH !"));
 lcdPrintString(1, 6, PSTR("V001"));
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuStatus() // show status
{
 lcdPrintString(0, 5, PSTR("STATUS")); // todo
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuLcdReport() // show lcd report in binary format
{
 lcdPrintString(0, 3, PSTR("Report LCD"));
 if (lcdReport.KeyEnter) lcdPrintString(1, 0, PSTR("ENT"));
 if (lcdReport.KeyHome) lcdPrintString(1, 3, PSTR("HOM"));
 if (lcdReport.KeyPlayPause) lcdPrintString(1, 6, PSTR("RUN"));
 if (lcdReport.KeyPlus) lcdPrintString(1, 9, PSTR(" + "));
 if (lcdReport.KeyMinus) lcdPrintString(1, 12, PSTR(" - "));

//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuDateTime() // show date time
{
 struct tm * utm = localtime((time_t*)&rtcTime);

 lcdPrintf(0, 3, PSTR("%02u/%02u/%u"), utm->tm_mday, utm->tm_mon + 1, utm->tm_year + 1900);
 lcdPrintf(1, 4, PSTR("%02u:%02u:%02u"), utm->tm_hour, utm->tm_min, utm->tm_sec);
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuFreeSram() // show free sram
{
 lcdPrintString(0, 3, PSTR("SRAM libre:"));
 u16 fs = freeSram();
 lcdPrintf(1, 3, PSTR("%04u Octets"), fs);
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuImuFusion() // Euler datas
{
 lcdPrintf(0,0,PSTR("R% 4i.%01i T% 4i.%01i"), ImuValues.roll/10, abs(ImuValues.roll)%10, ImuValues.pitch/10, abs(ImuValues.pitch)%10);
 lcdPrintf(1,0,PSTR("L% 4i.%01i N% 4i.%01i"), ImuValues.yaw/10, abs(ImuValues.yaw)%10, ImuValues.heading/10, abs(ImuValues.heading)%10);
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuImuGyro() // Gyroscope datas
{
 lcdPrintf(0,0,PSTR("Gyro : X% 4i.%01i"), imuGyro.x/10, abs(imuGyro.x)%10);
 lcdPrintf(1,0,PSTR("Y% 4i.%01i Z% 4i.%01i"), imuGyro.y/10, abs(imuGyro.y)%10, imuGyro.z/10, abs(imuGyro.z)%10);
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuImuAcc() // Accelerometer datas
{
 lcdPrintf(0,0,PSTR("Accel: X% 4i.%01i"), imuAcc.x/10, abs(imuAcc.x)%10);
 lcdPrintf(1,0,PSTR("Y% 4i.%01i Z% 4i.%01i"), imuAcc.y/10, abs(imuAcc.y)%10, imuAcc.z/10, abs(imuAcc.z)%10);
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}

void menuImuMag() // Accelerometer datas
{
 lcdPrintf(0,0,PSTR("Magne: X% 4i.%01i"), imuMag.x/10, abs(imuMag.x)%10);
 lcdPrintf(1,0,PSTR("Y% 4i.%01i Z% 4i.%01i"), imuMag.y/10, abs(imuMag.y)%10, imuMag.z/10, abs(imuMag.z)%10);
//menuNavigation(PMT(M_FIRST, M_STATUS, M_DATETIME, M_FIRST)); // Todo values for test
}
