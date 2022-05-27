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


#include "AnalogSensor.h"

//volatile u16 ana[256] = {0};
//volatile u8 anaCounter = 0;

void adcInit()
{
 ADMUX = ADC_VREF_TYPE;
 ADCSRA = _BV(ADEN) | ADC_PRESCALER16 ; // ADC enabled, no interrupt, no auto-triggering
 //ADCSRA = _BV(ADEN) | _BV(ADATE) | _BV(ADIE) | ADC_PRESCALER16 ; // ADC enabled, interrupt, auto-triggering
 //ADCSRB &= ~(_BV(ADTS2) | _BV(ADTS1) | _BV(ADTS0)); // continuous convertion
 DIDR0 = ANALOG_INPUT_MASK; // Digital input buffer disabled on unused ana pin.
}

u16 getADC(u8 input)
{
 ADMUX = input|ADC_VREF_TYPE;
 ADCSRA |= _BV(ADSC); // Start the AD conversion
 while bit_is_set(ADCSRA,ADSC); // Wait for the AD conversion to complete
 return ADC;
}

ISR(ADC_vect)
{
  //ana[anaCounter++] = ADC;
}


