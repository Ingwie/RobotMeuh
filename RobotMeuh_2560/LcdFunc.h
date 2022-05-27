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
/*       Copyright 2020-2022 by Ingwie (Bracame)       */
/*   Licence: GPLV3 see <http://www.gnu.org/licenses   */
/*        Compile with AVR GCC + Code::Blocks          */
/*    https://www.mediafire.com/file/cahqfrm90h7c7fy/  */
/*    Setup_OAVRCBuilder3.exe/file (Pswd : OpenAVRc)   */


#ifndef LCDFUNC_H_INCLUDED
#define LCDFUNC_H_INCLUDED

#include "RobotMeuh.h"

extern const char customChars[];

void lcdAction(lcdFunction action);

#define lcdDispOffClear() lcdAction(B_DispOff_Clear)
#define lcdClear()        lcdAction(B_Clear)
#define lcdDispOn()       lcdAction(B_DispOn)
#define lcdDispOff()      lcdAction(B_DispOff)
#define lcdLedOn()        lcdAction(B_LedOn)
#define lcdLedOff()       lcdAction(B_LedOff)
#define lcdBlinkOn()      lcdAction(B_BlinkOn)
#define lcdBlinkOff()     lcdAction(B_BlinkOff)
#define lcdLShift()       lcdAction(B_LShift)
#define lcdRShift()       lcdAction(B_RShift)
#define lcd2ndRow()       lcdAction(B_2ndRow)
#define lcdHome()         lcdAction(B_Home)
#define lcdCmdLoadCg()    lcdAction(B_CMD_LOAD_CGRAM)

#define sendLcdDispOffClear() lcdAction(B_DispOff_Clear);SerialLcdSend()
#define sendLcdClear()        lcdAction(B_Clear);SerialLcdSend()
#define sendLcdDispOn()       lcdAction(B_DispOn);SerialLcdSend()
#define sendLcdDispOff()      lcdAction(B_DispOff);SerialLcdSend()
#define sendLcdLedOn()        lcdAction(B_LedOn);SerialLcdSend()
#define sendLcdLedOff()       lcdAction(B_LedOff);SerialLcdSend()
#define sendLcdBlinkOn()      lcdAction(B_BlinkOn);SerialLcdSend()
#define sendLcdBlinkOff()     lcdAction(B_BlinkOff);SerialLcdSend()
#define sendLcdLShift()       lcdAction(B_LShift);SerialLcdSend()
#define sendLcdRShift()       lcdAction(B_RShift);SerialLcdSend()
#define sendLcd2ndRow()       lcdAction(B_2ndRow);SerialLcdSend()
#define sendLcdHome()         lcdAction(B_Home);SerialLcdSend()
#define sendLcdCmdLoadCg()    lcdAction(B_CmdLoadCg);SerialLcdSend()

void lcdPrintString(u8 row, u8 column, char *text);
void lcdPrintString_P(u8 row, u8 column, const char *text);
void lcdPrintf(u8 row, u8 column, PGM_P fmt, ...);
void lcdLocate(u8 row, u8 column);
void lcdPrintchar(char code);
void lcdLoadCgram(const char * tab, u8 charnum);

#endif // LCDFUNC_H_INCLUDED
