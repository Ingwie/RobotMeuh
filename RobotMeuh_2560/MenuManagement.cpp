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

void menuNavigation(PMT_t menuTarget)
{
 u8 tokenMem = menuToken;
 MenuTarget_t mt;

 memcpy_P(&mt, menuTarget, sizeof(MenuTarget_t)); // load targets values from flash

 if (lcdReport.KeyPlayPause)
  {
   menuToken = mt.PlayPause;
  }
 else if (lcdReport.KeyEnter)
  {
   menuToken = mt.Enter;
  }
 else if (lcdReport.KeyPlus)
  {
   menuToken = mt.Plus;
  }
 else if (lcdReport.KeyMinus)
  {
   menuToken = mt.Minus;
  }

 if (tokenMem != menuToken)
  {
   menuCompute();
  }
}

void menuCompute()
{
 lcdDispOffClear();
 MenuPointer = (p_Function)pgm_read_ptr_near(&MenuFunctions[menuToken]); // find menufunction[menuToken] in flash
 if (MenuPointer) MenuPointer(); // call it
 else ERR("pointeur nul");
 lcdDispOn();
}

void forceMenu(menuArray num)
{
 menuToken = num;
 menuCompute();
}
