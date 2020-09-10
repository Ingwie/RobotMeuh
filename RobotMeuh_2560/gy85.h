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

PACK(union gyro_t
{
 struct {
  int8_t xh;
  int8_t xl;
  int8_t yh;
  int8_t yl;
  int8_t zh;
  int8_t zl;
 } gbyte_t;
 struct {
  int16_t x;
  int16_t y;
  int16_t z;
 } gword_t;
});

extern gyro_t gyro;

PACK(union gyroTemp_t
{
 struct {
  int8_t h;
  int8_t l;
 } gbyte_t;
 struct {
  int16_t value;
 } gword_t;
});

extern gyroTemp_t gyroTemp;

void initGyro();
uint8_t readGyro(); // return 0 on success
uint8_t readGyroTemp(); // return 0 on success

#endif // GY85_H_INCLUDED
