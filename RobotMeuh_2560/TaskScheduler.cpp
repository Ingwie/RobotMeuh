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

#include "TaskScheduler.h"

void initTaskScheduler()
{
// Clear counter
 TCNT0 = 0;
// 8 mS 125 Hz (16000000/((124+1)*1024))
 OCR0A = 124;
// CTC
 TCCR0A = _BV(WGM01);
// Prescaler 1024 (15625 HZ - 64 uS)
 TCCR0B = _BV(CS02) | _BV(CS00);
// Output Compare Match A Interrupt Enable
 TIMSK0 |= _BV(OCIE0A);
}

ISR(TIMER0_COMPA_vect) // Timer 8mS
{
 if (++counter8mS >= 125) // 1 seconde
  {
   counter8mS = 0; // reset counter
   ++rtcTime; // increase time
   Task1S();  // conditional sei() in this function
   SystemBools.toggle500mS = 0;
  }
// enable interupts
 sei();
 if ((counter8mS & 0x07) == 0x04)
  {
// slow task 32mS
   Task32mS();
  }
 else
  {
// fast task 8mS
   Task8mS();
  }

 if ((counter8mS & 0x3E) == 0x3E) // 500 mS
  {
   SystemBools.toggle500mS = 1;
  }
}
