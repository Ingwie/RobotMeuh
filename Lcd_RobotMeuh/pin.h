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


#ifndef PIN_H_INCLUDED
#define PIN_H_INCLUDED

#define Unused1         D,0   // PD0


//-----> HD44780 Pin
/* LCDPort contains 4-bit data D0 to D3*/
/* LCDPort must be assigned to the chosen port */
/* PD4 to PD7 */
#define LCDPort         PORTD
#define LCDPin          PIND
#define LCDPinEnable    D,1   // PD1
#define LCDPinRS        D,2   // PD2
#define LCDPinPower     D,3   // PD3
#define LCDPinD4        D,4   // PD4
#define LCDPinD5        D,5   // PD5
#define LCDPinD6        D,6   // PD6
#define LCDPinD7        D,7   // PD7

#define Unused2         B,0   // PB0
#define Unused3         B,1   // PB1

//-----> SPI Pin
#define SpiSS           B,2   // PB2
#define SpiMosi         B,3   // PB3
#define SpiMiso         B,4   // PB4
#define SpiSck          B,5   // PB5

#endif // PIN_H_INCLUDED
