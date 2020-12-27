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

#ifndef PROTOCOL_H_INCLUDED
#define PROTOCOL_H_INCLUDED

#if defined(LCD_ROBOMEUH)
 #include "../Lcd_RobotMeuh_328p/Lcd_RobotMeuh.h"
#else
 #include "RobotMeuh.h"
#endif

#define SPI_BUFFER_LENGHT        20
#define SPI_BUFFER_NUM           10
#define SPI_EOT                  0x7F // DEL char

PACK(typedef struct
{
 u8 FistByteOne:1; // always 1
 u8 KeyPlayPause:1;
 u8 KeyHome:1;
 u8 KeyEnter:1;
 u8 KeyPlus:1;
 u8 KeyMinus:1;
 u8 DetectRain:1;
 u8 Todo:1;
}) DataLcdToMain_t;

PACK(typedef struct
{
 u8 IsRunning:1;
 u8 GoHome:1;
 u8 RunForward:1;
 u8 Unused:1;
 u8 RequestAction:4;
}) Status_t;

#define ISRUNNIG(x)        (x & 0x80)
#define GOHOME(x)          (x & 0x40)
#define RUNFORWARD(x)      (x & 0x20)
#define ISUNUSED(x)        (x & 0x10)

enum lcdActions   // 16 actions
{
 A_none = 0, // just return lcdReport
 A_lcdFunction,
 A_printString,
 A_rainTriggerValue,

};

enum lcdFunction   // 255 funcions
{
 B_DispOff_Clear,
 B_DispOn,
 B_Clear,
 B_DispOff,
 B_LedOn,
 B_LedOff,
 B_BlinkOn,
 B_BlinkOff,
 B_LShift,
 B_RShift,
 B_2ndRow,
 B_Home,

};

#endif // PROTOCOL_H_INCLUDED
