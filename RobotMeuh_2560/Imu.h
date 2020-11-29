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


#ifndef IMU_H_INCLUDED
#define IMU_H_INCLUDED

#include "RobotMeuh.h"

/*
ITG3205  - 0x68 — Three axis gyroscope
ADXL345 -  0x53 — Three axis acceleration
QMC5883L - 0x0D — Three axis magnetic field
*/

#define GYRO_RATE_XYZ      (1.0f/14.375f) // 14.375 LSBs per °/sec
#define ACC_RATE_XYZ       0.0039f        // 3.9mG
#define MAG_RATE_XYZ       (0.1f/12.0f)   //  12 lsb = 1mG . 1mG = 0.1 uT

struct imu_t
{
 int16_t x;
 int16_t y;
 int16_t z;
};

extern imu_t imuGyro;
extern imu_t imuAcc;
extern imu_t imuMag;

extern int16_t gyroTemp;

void initImus();

void initGyro();
uint8_t readGyro(); // return 0 on success
uint8_t readGyroTemp(); // return 0 on success

void initAcc();
uint8_t readAcc(); // return 0 on success

void initMag();
uint8_t readMag(); // return 0 on success

#endif // GY85_H_INCLUDED
