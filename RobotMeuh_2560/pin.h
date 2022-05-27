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


#ifndef PIN_H_INCLUDED
#define PIN_H_INCLUDED

//-----> Analog Pins
#define ANA_BAT            0 //K,0
#define ANA_CHARGE         1 //K,1

//-----> SPI Pin
//#define SpiLcdSSPin        B,0
//#define SpiSckPin          B,1
//#define SpiMosiPin         B,2
//#define SpiMisoPin         B,3
//-----

//-----> LCDSERIAL Pin
#define LcdSerialRxPin     D,17
#define LcdSerialTxPin     D,16

//-----> LCDCli Pin
#define CliSerialRxPin     J,00
#define CliSerialTxPin     J,01

//-----> I2C Pin
#define I2cSclPin          D,0
#define I2cSdaPin          D,1
//-----

//-----> Wheel (stepper) Pin
#define R_WheelEnablePin   A,0
#define L_WheelEnablePin   A,1
#define R_WheelDirPin      A,2
#define L_WheelDirPin      A,3
#define R_WheelPulsePin    E,4
#define L_WheelPulsePin    H,4
#define R_WheelPulseInPin  D,2
#define L_WheelPulseInPin  D,3
//-----

//-----> Blade (brushless) Pin
#define BladePulsesinPin   L,1
#define BladeClockwisePin  L,3
#define BladePwmOutPin     L,5
#define BladeEnablePin     L,7
//-----


#endif // PIN_H_INCLUDED
