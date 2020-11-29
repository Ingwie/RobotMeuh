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


#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "RobotMeuh.h"

#define FORCEINLINE inline __attribute__ ((always_inline))
#define NOINLINE __attribute__ ((noinline))

#define htons(x) __builtin_bswap16((uint16_t) (x))

template<class t> FORCEINLINE t min(t a, t b)
{
 return a<b?a:b;
}

template<class t> FORCEINLINE t max(t a, t b)
{
 return a>b?a:b;
}

template<class t> FORCEINLINE t sgn(t a)
{
 return a>0 ? 1 : (a < 0 ? -1 : 0);
}

template<class t> FORCEINLINE t limit(t mi, t x, t ma)
{
 return min(max(mi,x),ma);
}

template<class t> void SWAP(t & a, t & b)
{
 t tmp = b;
 b = a;
 a = tmp;
}

#define IS_IN_RANGE(value,low,high) (!(value<(low)) && (value<((high)+1)))

// BCD convertions
uint8_t bin2bcd(int8_t bin);
int8_t bcd2bin(uint8_t bcd);

#endif // UTILS_H_INCLUDED
