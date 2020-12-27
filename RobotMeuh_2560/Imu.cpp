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


#include "Imu.h"

void initImus() // init Gyro, accel and compass
{
 initGyro();
 initAcc();
 initMag();
}


// ITG3205 code --------------------------------------------------

#define GYRO_ADRESS      (0x68 << 1)

// Register 0x15 – Sample Rate Divider : SMPLRT_DIV
// F sample = 1Khz / (SMPLRT_DIV+1)
#define SMPLRT_DIV       0x1F // 32mS

// Register 0x16 – DLPF, Full Scale
#define FS_SEL           0x03
#define DLPF_CFG         0x03 // 42Hz low pass filter
#define DLPF             ((FS_SEL << 3) | DLPF_CFG)

// Registers 0x1B to 0x22 – Sensor Registers
#define TEMP_OUT_H       0x1B
#define TEMP_OUT_L       0x1C
#define GYRO_XOUT_H      0x1D
#define GYRO_XOUT_L      0x1E
#define GYRO_YOUT_H      0x1F
#define GYRO_YOUT_L      0x20
#define GYRO_ZOUT_H      0x21
#define GYRO_ZOUT_L      0x22

// Register 0x3E – Power Management
#define CLK_SEL          0x03 // PLL with Z Gyro reference

imu_t imuGyro;
s16 gyroTemp;

void initGyro()
{
 i2c_writeRegByte(GYRO_ADRESS, 0x15, SMPLRT_DIV); // Sample Rate Divider
 i2c_writeRegByte(GYRO_ADRESS, 0x16, DLPF); // low pass filter
 i2c_writeRegByte(GYRO_ADRESS, 0x3E, CLK_SEL); // frequency source
}

u8 readGyro() // return 0 on success
{
 u8 ret = i2c_readReg(GYRO_ADRESS, GYRO_XOUT_H, (u8*)&imuGyro, 6);
// swap bytes
 imuGyro.x = htons(imuGyro.x);
 imuGyro.y = htons(imuGyro.y);
 imuGyro.z = htons(imuGyro.z);
return ret;
}

u8 readGyroTemp() // return 0 on success
{
 u8 ret = i2c_readReg(GYRO_ADRESS, TEMP_OUT_H, (u8*)&gyroTemp, 2);
 gyroTemp = htons(gyroTemp);
 gyroTemp = ((s32) 35 + (gyroTemp + 13200) / 280);
 return ret;
}


// ADXL345 code --------------------------------------------------

#define ACC_ADRESS      (0x53 << 1)

//Register 0x2C—BW_RATE
#define BW_RATE         0x2C
/*
Output Data
Rate (Hz) Bandwidth (Hz) Rate Code
400         200            1100
200         100            1011
100         50             1010
50          25             1001
25          12.5           1000
12.5        6.25           0111
*/
#define RATE            0x09

//Register 0x2D—POWER_CTL
#define POWER_CTL       0x2D
#define ACCMeasure      _BV(3)

//Register 0x31—DATA_FORMAT
#define DATA_FORMAT     0x31
#define FULL_RES        _BV(3)
#define SET16G          0x03

// Data
#define DATAX0          0x32

imu_t imuAcc;

void initAcc()
{
 i2c_writeRegByte(ACC_ADRESS, BW_RATE, RATE);
 i2c_writeRegByte(ACC_ADRESS, POWER_CTL, ACCMeasure);
 i2c_writeRegByte(ACC_ADRESS, DATA_FORMAT, FULL_RES | SET16G);
}

u8 readAcc() // return 0 on success
{
 return i2c_readReg(ACC_ADRESS, DATAX0, (u8*)&imuAcc, 6);
}


// QMC5883L code --------------------------------------------------

#define MAG_ADRESS      (0x0D << 1)

#define CTRLREG1        0x09
#define CTRLREG1VAL     0b00000101 // 50Hz, High filter, 2 Gauss

#define CTRLREG2        0x0A
#define ROL_PNT         _BV(6)     // To enable pointer roll-over function

#define SETRESETREG     0x0B
#define SETRESETREGVAL  0x01

imu_t imuMag;

void initMag()
{
 i2c_writeRegByte(MAG_ADRESS, CTRLREG1, CTRLREG1VAL);
 i2c_writeRegByte(MAG_ADRESS, CTRLREG2, ROL_PNT);
 i2c_writeRegByte(MAG_ADRESS, SETRESETREG, SETRESETREGVAL);
}

u8 readMag() // return 0 on success
{
 return i2c_readReg(MAG_ADRESS, 0x00, (u8*)&imuMag, 6);
}
