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


#ifndef LCDFUNC_H_INCLUDED
#define LCDFUNC_H_INCLUDED

#include "RobotMeuh.h"

void lcdAction(lcdFunction action);

#define lcdDispOffClear() lcdAction(B_DispOff_Clear)
#define lcdClear()        lcdAction(B_Clear)
#define lcdDispOn()       lcdAction(B_DispOn)
#define lcdDispOff()      lcdAction(B_DispOff)
#define lcdLedOn()        lcdAction(B_LedOn)
#define lcdLedOff()       lcdAction(B_LedOff)
#define lcdLShift()       lcdAction(B_LShift)
#define lcdRShift()       lcdAction(B_RShift)
#define lcd2ndRow()       lcdAction(B_2ndRow)
#define lcdHome()         lcdAction(B_Home)

void lcdPrintString(u8 row, u8 column, const char *text);
void lcdPrintf(u8 row, u8 column, PGM_P fmt, ...);


#endif // LCDFUNC_H_INCLUDED
