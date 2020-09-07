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

#include "Timer8mS.h"

void initTimer8mS()
{
// TIMER 0 for interrupt frequency 125 Hz / 8mS:
 cli(); // stop interrupts
 TCCR0A = 0; // set entire TCCR0A register to 0
 TCCR0B = 0; // same for TCCR0B
 TCNT0  = 0; // initialize counter value to 0
// set compare match register for 125 Hz increments
 OCR0A = 124; // = 16000000 / (1024 * 125) - 1 (must be <256)
// turn on CTC mode
 TCCR0B |= (1 << WGM01);
// Set CS02, CS01 and CS00 bits for 1024 prescaler
 TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00);
// enable timer compare interrupt
 TIMSK0 |= (1 << OCIE0A);
 sei(); // allow interrupts
}

ISR(TIMER0_COMPA_vect, ISR_NOBLOCK)
{
 static uint8_t tik = 0;
 if (++tik == TIKTIMEOUT)
 {
   tik = 0;
   //Update Report for keys
   updateKeys();
   cli();
   memcpy(&SpiRet, &Report, 1); // Update Spiret IRQ mode
   sei();
 }
}
