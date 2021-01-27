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

#include "SerialLcd.h"

#define USART_LCD_ID                0
#define SERIAL_LCD_RX_BUFFER_NUM    6
#define SERIAL_LCD_RX_BUFFER_LENGHT SERIAL_LCD_BUF_LENGHT
#define SERIAL_LCD_TX_BUFFER_NUM    2
#define SERIAL_LCD_TX_BUFFER_LENGHT NUM_BYTE_RET


// RX buffer
char SerialLcdRXBuf[SERIAL_LCD_RX_BUFFER_NUM][SERIAL_LCD_RX_BUFFER_LENGHT] = {SERIAL_LCD_EOL};
volatile u8 SerialLcdRXBufWrite = 0;
u8 SerialLcdRXBufRead = 0;
// TX buffer
char SerialLcdTXBuf[SERIAL_LCD_TX_BUFFER_NUM][SERIAL_LCD_TX_BUFFER_LENGHT] = {SERIAL_LCD_EOL};
u8 SerialLcdTXBufWrite = 0;
volatile u8 SerialLcdTXBufRead = 0;

void initSerialLcd()
{
// Pin
 set_input(SerialRxPin);
 set_output_off(SerialTxPin);
// set baudrate
#undef BAUD
#define BAUD SERIAL_LCD_SPEED
#include <util/setbaud.h>
 UBRRH_N(USART_LCD_ID) = UBRRH_VALUE;
 UBRRL_N(USART_LCD_ID) = UBRRL_VALUE;
#if USE_2X
 UCSRA_N(USART_LCD_ID) |= (1 << U2X_N(USART_LCD_ID));
#else
 UCSRA_N(USART_LCD_ID) &= ~(1 << U2X_N(USART_LCD_ID));
#endif
// set 8N1
 UCSRB_N(USART_LCD_ID) = (0 << RXCIE_N(USART_LCD_ID)) | (0 << TXCIE_N(USART_LCD_ID)) | (0 << UDRIE_N(USART_LCD_ID)) | (0 << RXEN_N(USART_LCD_ID)) | (0 << TXEN_N(USART_LCD_ID)) | (0 << UCSZ2_N(USART_LCD_ID));
 UCSRC_N(USART_LCD_ID) = (1 << UCSZ1_N(USART_LCD_ID)) | (1 << UCSZ0_N(USART_LCD_ID)); // Set 1 stop bit, No parity bit.
// enable RX
 UCSRB_N(USART_LCD_ID) |= (1 << RXEN_N(USART_LCD_ID));  // enable RX
 UCSRB_N(USART_LCD_ID) |= (1 << RXCIE_N(USART_LCD_ID)); // enable Interrupt
 while (UCSRA_N(USART_LCD_ID) & (1 << RXC_N(USART_LCD_ID))) UDR_N(USART_LCD_ID); // Flush RX buffer.
// enable TX
 UCSRB_N(USART_LCD_ID) |= (1 << TXEN_N(USART_LCD_ID)); // enable TX
}

void computeSerialRXBuf(u8 bufferNum)
{
 memcpy(&RobotStatus, &SerialLcdRXBuf[bufferNum][0], 1);

 switch (RobotStatus.RequestAction)
  {
  case A_lcdFunction :
   switch (SerialLcdRXBuf[bufferNum][1])
    {
    case B_DispOff_Clear :
     lcdClear();
     lcdDispOff();
     break;
    case  B_DispOn :
     lcdDispOn();
     break;
    case B_Clear :
     lcdClear();
     break;
    case  B_DispOff :
     lcdDispOff();
     break;
    case  B_BlinkOn :
     lcdBlinkOn();
     break;
    case  B_BlinkOff :
     lcdBlinkOff();
     break;
    case  B_LedOn :
     lcdLedOn();
     break;
    case  B_LedOff :
     lcdLedOff();
     break;
    case  B_LShift :
     lcdLShift();
     break;
    case  B_RShift :
     lcdRShift();
     break;
    case  B_2ndRow :
     lcd2ndRow();
     break;
    case  B_Home :
     lcdHome();
     break;
    case  B_CmdLoadCg :
     lcdCmd(CGRAM_address_start);
     break;

    }
   break;

  case A_locate :
   lcdLocate((SerialLcdRXBuf[bufferNum][1] >> 4), (SerialLcdRXBuf[bufferNum][1] & 0xF));
   break;

  case A_printChar :
   lcdPrintchar(SerialLcdRXBuf[bufferNum][1]);
   break;

  case A_printString :
   lcd_printStringAt((SerialLcdRXBuf[bufferNum][1] >> 4), (SerialLcdRXBuf[bufferNum][1] & 0xF), &SerialLcdRXBuf[bufferNum][2]);
   break;

  case A_rainTriggerValue :
   //todo
   break;

  default : // case A_none ...
   break;
  }
}

void SerialLcdPrint(u8 * text)
{
 memcpy(SerialLcdTXBuf[SerialLcdTXBufWrite], text, NUM_BYTE_RET); // copy to TX buffer
 if(++SerialLcdTXBufWrite >= SERIAL_LCD_TX_BUFFER_NUM) // Change buffer
  {
   SerialLcdTXBufWrite = 0;
  }
}

void checkSerialLcdRXBuf()
{
 uint8_t cont = 1;
 do
  {
   if (SerialLcdRXBufRead != SerialLcdRXBufWrite) // there is some datas arrived
    {
     computeSerialRXBuf(SerialLcdRXBufRead); // Compute them !
     if(++SerialLcdRXBufRead >= SERIAL_LCD_RX_BUFFER_NUM) SerialLcdRXBufRead = 0; // Change buffer
    }
   else
    {
     cont = 0;
    }
  }
 while (cont);
}

void SerialLcdSend()
{
 if (SerialLcdTXBufWrite != SerialLcdTXBufRead) // there is some datas to send
  {
   UCSRB_N(USART_LCD_ID) |= (1 << UDRIE_N(USART_LCD_ID)); // enable Data Register Empty Interrupt
  }
}

ISR(USART_RX_vect)
{
 static u8 SerialLcdRXBufCount = 0;

 u8 stat = UCSRA_N(USART_LCD_ID); // USART control and Status Register 0/1 A

 if (stat & ((1 << FE_N(USART_LCD_ID)) | (1 << DOR_N(USART_LCD_ID)) | (1 << UPE_N(USART_LCD_ID))))
  {
   // discard buffer and start fresh on any comms error
   SerialLcdRXBufCount = 0; // reset counter
  }
 else
  {
   u8 data = UDR_N(USART_LCD_ID); // USART data register 0
   if (data != SERIAL_LCD_EOL)
    {
     SerialLcdRXBuf[SerialLcdRXBufWrite][SerialLcdRXBufCount++] = data;
    }
   else
    {
     SerialLcdRXBuf[SerialLcdRXBufWrite][SerialLcdRXBufCount++] = 0; // end of string
     if(++SerialLcdRXBufWrite >= SERIAL_LCD_RX_BUFFER_NUM)
      {
       SerialLcdRXBufWrite = 0; // Change buffer
      }
     SerialLcdRXBufCount = 0; // reset counter
    }
  }
}

// USART0 Transmit Data Register Emtpy ISR (UDR was loaded in Shift Register)
ISR(USART_UDRE_vect)
{
 static u8 SerialLcdTXBufCount = 0;

 if (SerialLcdTXBufWrite != SerialLcdTXBufRead) // there is some datas to send
  {
   char data = SerialLcdTXBuf[SerialLcdTXBufRead][SerialLcdTXBufCount++];
   UDR_N(USART_LCD_ID) = data; // send data
   if (data == SERIAL_LCD_EOL)
    {
     if(++SerialLcdTXBufRead >= SERIAL_LCD_TX_BUFFER_NUM) SerialLcdTXBufRead = 0; // Change buffer
     SerialLcdTXBufCount = 0; // reset counter
    }
  }
 else
  {
   UCSRB_N(USART_LCD_ID) &= ~(1 << UDRIE_N(USART_LCD_ID)); // Disable UDRE interrupt.
  }
}
