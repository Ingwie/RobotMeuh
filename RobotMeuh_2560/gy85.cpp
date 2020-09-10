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

gyro_t gyro;
gyroTemp_t gyroTemp;

void initGyro()
{
 I2C_SPEED_400K(); // TODO : Test faster
 i2c_writeRegByte(GYRO_ADRESS, 0x15, SMPLRT_DIV); // Sample Rate Divider
 i2c_writeRegByte(GYRO_ADRESS, 0x16, DLPF); // low pass filter
 i2c_writeRegByte(GYRO_ADRESS, 0x3E, CLK_SEL); // frequency source
}

uint8_t readGyro() // return 0 on success
{
  return i2c_readReg(GYRO_ADRESS, GYRO_XOUT_H, (uint8_t*)&gyro, 6); // todo div 14.375 (115/8)
}

uint8_t readGyroTemp() // return 0 on success
{
  return i2c_readReg(GYRO_ADRESS, TEMP_OUT_H, (uint8_t*)&gyroTemp, 2); // 35 + (gyroTemp.value + 13200)) / 280;
}
