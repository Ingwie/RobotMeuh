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

#include "MenuManagement.h"

menuArray menuToken;

p_Function MenuPointer;

MenuVar_t menuVar = {0};

void menuNavigation(PMT_t menuTarget)
{
 MenuTarget_t mt;

 if (!menuVar.editMode) // edit mode -> exit
  {
   memcpy_P(&mt, menuTarget, sizeof(MenuTarget_t)); // load targets values from flash

   if (lcdReport.KeyHome) menuToken = M_STATUS;
   else if (lcdReport.KeyPlay) menuToken = mt.Play;
   else if (lcdReport.KeyEnter) menuToken = mt.Enter;
   else if (lcdReport.KeyPlus) menuToken = mt.Plus;
   else if (lcdReport.KeyMinus) menuToken = mt.Minus;
   if (!IS_IN_RANGE(menuToken,M_FIRST,M_MENUNUMBER))
    {
     ERR("Menu inconnu");
     menuToken = M_FIRST;
     // StopAll(); todo
    }
  }
  else
  {
    if (!menuVar.maxField) menuVar.editMode = 0; // no editable field -> reset edit mode
  }
}

s16 setMenuValue(s16 actual, s16 maxi, s16 mini, s16 step)
{
// field
 if (lcdReport.KeyPlay)
  {
   if (menuVar.field < menuVar.maxField) ++menuVar.field;
   else menuVar.editMode = 0;
  }
 if (lcdReport.KeyEnter)
  {
   if (menuVar.field > 1) --menuVar.field;
   else menuVar.editMode = 0;
  }
// value
 s16 valueMem = actual;
 if (lcdReport.KeyHome) step *= 100; // increase step
 if (lcdReport.KeyMinus)
  {
   if (actual >= (mini + step)) actual -= step;
   else  actual = mini;
  }
 if (lcdReport.KeyPlus)
  {
   if (actual <= (maxi - step)) actual += step;
   else actual = maxi;
  }
 memset(&lcdReport, 0, sizeof(lcdReport)); // reset keys
 if(valueMem != actual) menuVar.wasEdited = 1;
 return actual;
}

void menuCompute()
{
 static u8 tokenMem = 0;

 if (menuToken != tokenMem) // change menu ?
  {
   tokenMem = menuToken;
   memset(&menuVar, 0, sizeof(menuVar)); // reset edit mode, field and value
  }
 else
  {
   if (lcdReport.KeyPlus && lcdReport.KeyMinus)
    {
     menuVar.editMode = !menuVar.editMode; // edit (field)value
     if (menuVar.editMode) menuVar.field = 1; // set first field
    }
  }
 lcdDispOffClear();
 MenuPointer = (p_Function)pgm_read_ptr_near(&MenuFunctions[menuToken]); // find menufunction[menuToken] in flash
 if (MenuPointer) MenuPointer(); // call it
 else ERR("pointeur nul");

 if (!SystemBools.lcdISOk) lcdPrintString_P(0, 0, PSTR("PB ecran")); // todo remove

 (menuVar.editMode)? lcdBlinkOn() : lcdDispOn(); // blink on edit mode

 SerialLcdSend(); // send serial buffer
}

void forceMenu(menuArray num)
{
 menuToken = num;
 menuCompute();
}
