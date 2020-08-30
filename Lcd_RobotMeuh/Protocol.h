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

#include "Lcd_RobotMeuh.h"

PACK(typedef struct
{
 uint8_t KeyPlayPause:1;
 uint8_t KeyHome:1;
 uint8_t KeyEnter:1;
 uint8_t KeyPlus:1;
 uint8_t KeyMinus:1;
 uint8_t DetectRain:1;
 uint8_t DetectUsR:1;
 uint8_t DetectUsL:1;
}) DataToSend_t;

PACK(typedef struct
{
 uint8_t IsRunnig:1;
 uint8_t GoHome:1;
 uint8_t Direction:1;
 uint8_t Unused:1;
 uint8_t RequestAction:4;
}) Status_t;

#define ISRUNNIG(x)        (x & 0x80)
#define GOHOME(x)          (x & 0x40)
#define RUNFORWARD(x)      (x & 0x20)
#define ISUNUSED(x)        (x & 0x10)

enum actions   // 16 actions
{
 none = 0, // just return Report
 printString,
 rainTriggerValue,

};

#endif // PROTOCOL_H_INCLUDED
