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


#include "gy85.h"

// ITG3205 code --------------------------------------------------

#define GYRO_ADRESS      0x69

// Register 0x15 – Sample Rate Divider : SMPLRT_DIV
// F sample = 1Khz / (SMPLRT_DIV+1)
#define SMPLRT_DIV       0x00

// Register 0x16 – DLPF, Full Scale
#define FS_SEL           0x03
#define DLPF_CFG         0x04 // 20Hz low pass filter
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

imu_t gyro;
int16_t gyroTemp;

void initGyro()
{
 I2C_SPEED_GIRO();
 i2c_writeRegByte(GYRO_ADRESS, 0x15, SMPLRT_DIV); // Sample Rate Divider
 i2c_writeRegByte(GYRO_ADRESS, 0x16, DLPF); // low pass filter
 i2c_writeRegByte(GYRO_ADRESS, 0x3E, CLK_SEL); // frequency source
}

uint8_t readGyro() // return 0 on success
{
 I2C_SPEED_GIRO();
 return i2c_readReg(GYRO_ADRESS, GYRO_XOUT_H, (uint8_t*)&gyro, 6);
}

uint8_t readGyroTemp() // return 0 on success
{
 I2C_SPEED_GIRO();
 uint8_t ret = i2c_readReg(GYRO_ADRESS, TEMP_OUT_H, (uint8_t*)&gyroTemp, 2);
 gyroTemp = ((int32_t) 35 + (gyroTemp + 13200) / 280);
 return ret;
}


// ADXL345 code --------------------------------------------------

#define ACC_ADRESS      0x53

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

imu_t acc;

void initAcc()
{
 I2C_SPEED_ACC();
 i2c_writeRegByte(ACC_ADRESS, BW_RATE, RATE);
 i2c_writeRegByte(ACC_ADRESS, POWER_CTL, ACCMeasure);
 i2c_writeRegByte(ACC_ADRESS, DATA_FORMAT, FULL_RES | SET16G);
}

uint8_t readAcc() // return 0 on success
{
 I2C_SPEED_ACC();
 return i2c_readReg(ACC_ADRESS, DATAX0, (uint8_t*)&acc, 6);
}


// HMC5883L code --------------------------------------------------

#define MAG_ADRESS      0x1E

//Configuration Register A
#define REGCONA         0x00
#define MA              (0x2 << 5) // Num sample (8)
#define DO              (0x4 << 2) // Output rate (15hz)

//Configuration Register B
#define REGCONB         0x01
#define GN              (0x0 << 5) // Gain 0.73 mG/Lsb 0,073 uT/Lsb
//Mode Register
#define REGMODE         0x02
#define HS              _BV(7) // Hight speed I2C ?? To test

#define XMSBRegister    0x03

imu_t mag;

void initMag()
{
 I2C_SPEED_MAG();
 i2c_writeRegByte(MAG_ADRESS, REGCONA, MA | DO);
 i2c_writeRegByte(MAG_ADRESS, REGCONA, GN);
 i2c_writeRegByte(MAG_ADRESS, REGMODE, 0x00 /* | HS*/);
}

uint8_t readMag() // return 0 on success
{
 I2C_SPEED_MAG();
 return i2c_readReg(MAG_ADRESS, XMSBRegister, (uint8_t*)&mag, 6);
}
