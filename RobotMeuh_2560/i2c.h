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



#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

#include "RobotMeuh.h"

#define I2C_READ          0x01
#define I2C_WRITE         0x00

#define I2C_SPEED_100K()  { TWBR = (u8) 72;}
#define I2C_SPEED_400K()  { TWBR = (u8) 12;}
#define I2C_SPEED_888K()  { TWBR = (u8) 1; }
#define I2C_SPEED_1M()    { TWBR = (u8) 0; }

void initI2C();
u8 i2c_start(u8 address);
u8 i2c_write(u8 data);
void i2c_writeAndActiveISR(u8 data);
u8 i2c_read_ack();
u8 i2c_read_nack();
u8 i2c_transmit(u8 address, u8* data, u16 length);
u8 i2c_receive(u8 address, u8* data, u16 length);
u8 i2c_writeReg(u8 devaddr, u8 regaddr, u8* data, u16 length);
u8 i2c_writeRegByte(u8 devaddr, u8 regaddr, u8 data);
u8 i2c_readReg(u8 devaddr, u8 regaddr, u8* data, u16 length);
void i2c_stop();

#endif // I2C_H_INCLUDED
