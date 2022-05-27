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


#ifndef PIN_HELPER_H_INCLUDED
#define PIN_HELPER_H_INCLUDED

//******************************************************************************************//
//                                                                                          //
//                           Macros for easy i/o pin access                                 //
//            From https://www.avrfreaks.net/forum/macros-ddr-and-port-pin-name             //
//                       Modded and updated by Ingwie (c) 2020                              //
//                                                                                          //
//******************************************************************************************//


#define BIT(p,b)                (b)

#define PORT(p,b)               (PORT ## p)
#define PIN(p,b)                (PIN ## p)
#define DDR(p,b)                (DDR ## p)


#define Set_Port_Bit(p,b)       ((p) |= _BV(b))
#define Clr_Port_Bit(p,b)       ((p) &= ~_BV(b))
#define Tgl_Port_Bit(p,b)       ((p) ^= _BV(b))

#define Get_Port_Bit(p,b)       (((p) & _BV(b)) != 0)


//user functions:
#define bit(io)	                BIT(io)
#define port(io)                PORT(io)
#define mask(io)                _BV(BIT(io))

#define pin_high(io)            Set_Port_Bit(PORT(io),BIT(io))
#define pin_low(io)             Clr_Port_Bit(PORT(io),BIT(io))
#define pin_toggle(io)          Tgl_Port_Bit(PORT(io),BIT(io))

#define get_output(io)          Get_Port_Bit(PORT(io),BIT(io))
#define get_input(io)           Get_Port_Bit(PIN(io),BIT(io))

#define set_dir_in(io)          (Clr_Port_Bit(DDR(io),BIT(io)))
#define set_dir_out(io)         (Set_Port_Bit(DDR(io),BIT(io)))
#define dir_toggle(io)          (Tgl_Port_Bit(DDR(io),BIT(io)))

#define set_input_pullup(io)      \
 Set_Port_Bit(PORT(io),BIT(io));  \
 (Clr_Port_Bit(DDR(io),BIT(io)))  \

#define set_input(io)             \
 Clr_Port_Bit(PORT(io),BIT(io));  \
 (Clr_Port_Bit(DDR(io),BIT(io)))  \

#define set_output_on(io)         \
 Set_Port_Bit(PORT(io),BIT(io));  \
 (Set_Port_Bit(DDR(io),BIT(io)))  \

#define set_output_off(io)        \
 Clr_Port_Bit(PORT(io),BIT(io));  \
 (Set_Port_Bit(DDR(io),BIT(io)))  \

#endif // PIN_HELPER_H_INCLUDED
