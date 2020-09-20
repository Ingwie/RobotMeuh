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

#include "StepperWheel.h"

void initStepperWeel()
{
 set_output_off(R_WheelEnablePin);
 set_output_off(L_WheelEnablePin);
 set_output_off(R_WheelDirPin);
 set_output_off(L_WheelDirPin);
 set_output_off(R_WheelPulsePin);
 set_output_off(L_WheelPulsePin);
// Setup Timer 1.
// PB5 OC1A drive left motor.
// PB6 OC1B drive right motor.
 TCCR1A = 0; // Normal mode (0), OVF @ TOP (0xFFFF)
 TCCR1C = 0;
 TCCR1B = 0; // Disabled.
}

void enableStepperWheel()
{
 pin_high(R_WheelEnablePin);
 pin_high(L_WheelEnablePin);
 uint8_t sreg = SREG;
 cli();
 TCNT1 = 0; // Reset timer value
 TCCR1B |= (_BV(CS11) | _BV(CS10)); // Enabled : F_CPU/64 -> 4uS tick.
 TIMSK1 |= (_BV(OCIE1A) | _BV(OCIE1B)); // Enable Output Compare A & B interrupt.
 SREG = sreg;
}

void disableStepperWheel()
{
 pin_low(R_WheelEnablePin);
 pin_low(L_WheelEnablePin);
 pin_low(R_WheelPulsePin);
 pin_low(L_WheelPulsePin);
 TCCR1B = (_BV(CS11) | _BV(CS10)); // Disabled
 TIMSK1 &= ~(_BV(OCIE1A) | _BV(OCIE1B)); // Disable Output Compare A & B interrupt.
}

void stopStepperWheel()
{
 TCCR1B = (_BV(CS11) | _BV(CS10)); // Disabled
 TIMSK1 &= ~(_BV(OCIE1A) | _BV(OCIE1B)); // Disable Output Compare A & B interrupt.
}

ISR(TIMER1_COMPA_vect) // left motor
{
 pin_toggle(L_WheelPulsePin);
 OCR1A += 0; // todo
}

ISR(TIMER1_COMPB_vect) // right motor
{
 pin_toggle(R_WheelPulsePin);
 OCR1B += 0; // todo
}
