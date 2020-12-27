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

#define UnusedT        D,0   // PD0
#define UnusedX        D,1   // PD1


//-----> HD44780 Pin
/* LCDPort contains 4-bit data D0 to D3*/
/* LCDPort must be assigned to the chosen port */
/* PD4 to PD7 */
#define LCDPort         PORTD
#define LCDPinRS        D,2   // PD2
#define LCDPinEnable    D,3   // PD3
#define LCDPinD4        D,4   // PD4
#define LCDPinD5        D,5   // PD5
#define LCDPinD6        D,6   // PD6
#define LCDPinD7        D,7   // PD7
#define LCDPinLed       B,0   // PB0
//-----

#define Unused3         B,1   // PB1

//-----> SPI Pin
#define SpiSSPin        B,2   // PB2
#define SpiMosiPin      B,3   // PB3
#define SpiMisoPin      B,4   // PB4
#define SpiSckPin       B,5   // PB5
//-----

//-----> KEYBOARD Pin
#define KeyHomePin      C,0   //PC0
#define KeyPlayPausePin C,1   //PC1
#define KeyEnterPin     C,2   //PC2
#define KeyMinusPin     C,3   //PC3
#define KeyPlusPin      C,4   //PC4
//-----

#define Unused4         C,5   //PC5
#define Reset           C,6   //PC6

//-----> ANALOG Pin
#define RainInput       6
#define UnusedAna1      7
#define TemSensor       8
//-----

#endif // PIN_H_INCLUDED
