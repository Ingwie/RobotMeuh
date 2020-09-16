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


#ifndef GY85_H_INCLUDED
#define GY85_H_INCLUDED

#include "RobotMeuh.h"

/*
ITG3205  - 0x69 — Three axis gyroscope
ADXL345 -  0x53 — Three axis acceleration
HMC5883L - 0x1E — Three axis magnetic field
*/
#define I2C_SPEED_GIRO()   I2C_SPEED_400K() // TODO : Test faster
#define I2C_SPEED_ACC()    I2C_SPEED_400K() // TODO : Test faster
#define I2C_SPEED_MAG()    I2C_SPEED_400K() // TODO : Test faster

#define GYRO_RATE_XYZ      0.06956521739130434782608695652174f
#define ACC_RATE_XYZ       0.0039f
#define MAG_RATE_XYZ       0,073f // uT/Lsb || mG/(10*Lsb)

struct imu_t
{
 int16_t x;
 int16_t y;
 int16_t z;
};

extern imu_t gyro;
extern imu_t acc;
extern imu_t mag;


extern int16_t gyroTemp;

void initGyro();
uint8_t readGyro(); // return 0 on success
uint8_t readGyroTemp(); // return 0 on success

void initAcc();
uint8_t readAcc(); // return 0 on success

void initMag();
uint8_t readMag(); // return 0 on success

#endif // GY85_H_INCLUDED
