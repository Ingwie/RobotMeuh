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
 TCCR2A = 0; // set entire TCCR0A register to 0
 TCCR2B = 0; // same for TCCR0B
 TCNT2  = 0; // initialize counter value to 0
// set compare match register for 125 Hz increments
 OCR2A = 124; // = 16000000 / (1024 * 125) - 1 (must be <256)
// turn on CTC mode
 TCCR2B |= (1 << WGM21);
// enable timer compare interrupt
 TIMSK2 |= (1 << OCIE2A);
// Set CS02, CS01 and CS00 bits for 1024 prescaler
 TCCR2B |= (1 << CS22) | (0 << CS21) | (1 << CS20);
}

ISR(TIMER2_COMPA_vect, ISR_NOBLOCK)
{
 static u8 tik = 0;
 if (++tik >= TIKTIMEOUT)
  {
   tik = 0;
   // Update Report for keys
   updateKeys();
   // update heartbeat
   Report.heartbeat = (Report.heartbeat)? 0 : 1;

   memcpy(&serialRet, &Report, 1); // Update Spiret
   serialRet[NUM_BYTE_RET - 1] = SERIAL_LCD_EOL; // end of packet
   SerialLcdPrint(serialRet);
   SerialLcdSend();
  }
}
