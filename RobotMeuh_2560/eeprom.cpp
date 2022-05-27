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

#include "eeprom.h"

void eepromReadAll() // load RobotMeuh settings
{
 u8 * P = (u8 *)&RobotMeuh; // load datas address in Sram

 for (u16 i = 0; i < sizeof(RobotMeuh_t); i++)
  {
   while(EECR & (1<<EEPE));
   EEAR = i; // set eeprom address
   EECR |= _BV(EERE); // read value
   *(P++) = EEDR; // write value in Sram
  }
}

inline void eepromWriteOneByteIfDifferent(u8 data, u16 addr) // Write one byte is needed
{
// This function is rewrited from the fantastic GRBL one (https://github.com/gnea/grbl-Mega)
// Eeprom Write/Erase Cycles limit increased

 while(EECR & _BV(EEPE)); // wait previous write completion

 EEAR = addr; // set address
 EECR = _BV(EERE); // read
 u8 actualData = EEDR; // store actual eeprom value
 u8 diff = actualData ^ data; // Get bit differences.
// Check if any bits are changed to '1' in the new value.
 if(diff & data)
  {
   // Now we know that _some_ bits need to be erased to '1'.
   // Check if any bits in the new value are '0'.
   if(data != 0xff)
    {
     // Now we know that some bits need to be programmed to '0' also.
     EEDR = data; // Set EEPROM data register.
     EECR = _BV(EEMPE);  // Set Master Write Enable bit and Erase+Write mode.
     EECR |= _BV(EEPE);  // Start Erase+Write operation.
    }
   else
    {
     // Now we know that all bits should be erased.
     EECR = _BV(EEMPE) | _BV(EEPM0); // Set Master Write Enable bit and Erase-only mode.
     EECR |= _BV(EEPE);  // Start Erase-only operation.
    }
  }
 else
  {
   // Now we know that _no_ bits need to be erased to '1'.
   // Check if any bits are changed from '1' in the old value.
   if(diff)
    {
     // Now we know that _some_ bits need to the programmed to '0'.
     EEDR = data;   // Set EEPROM data register.
     EECR = _BV(EEMPE) | // Set Master Write Enable bit...
            _BV(EEPM1);  // ...and Write-only mode.
     EECR |= _BV(EEPE);  // Start Write-only operation.
    }
  }
}

void eepromWritedAll() // write RobotMeuh settings
{
 u8 * P = (u8 *)&RobotMeuh; // load datas address in Sram
 cli();
 for (u16 i = 0; i < sizeof(RobotMeuh_t); i++)
  {
   eepromWriteOneByteIfDifferent(*(P++), i);
  }
 sei();
}
