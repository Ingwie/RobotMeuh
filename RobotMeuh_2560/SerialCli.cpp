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

#include "SerialCli.h"

#define USART_CLI_ID                3
#define SERIAL_CLI_EOL              0x0D // CR
#define SERIAL_CLI_RX_BUFFER_NUM    2
#define SERIAL_CLI_RX_BUFFER_LENGHT 8
#define SERIAL_CLI_TX_BUFFER_NUM    4
#define SERIAL_CLI_TX_BUFFER_LENGHT 17


// RX buffer
char SerialCliRXBuf[SERIAL_CLI_RX_BUFFER_NUM][SERIAL_CLI_RX_BUFFER_LENGHT] = {SERIAL_CLI_EOL};
volatile u8 SerialCliRXBufWrite = 0;
u8 SerialCliRXBufRead = 0;
// TX buffer
char SerialCliTXBuf[SERIAL_CLI_TX_BUFFER_NUM][SERIAL_CLI_TX_BUFFER_LENGHT] = {SERIAL_CLI_EOL};
u8 SerialCliTXBufWrite = 0;
volatile u8 SerialCliTXBufRead = 0;

void initSerialCli()
{
// Pin
 set_input(CliSerialRxPin);
 set_output_off(CliSerialTxPin);
// set 8N1
 UCSRB_N(USART_CLI_ID) = (0 << RXCIE_N(USART_CLI_ID)) | (0 << TXCIE_N(USART_CLI_ID)) | (0 << UDRIE_N(USART_CLI_ID)) | (0 << RXEN_N(USART_CLI_ID)) | (0 << TXEN_N(USART_CLI_ID)) | (0 << UCSZ2_N(USART_CLI_ID));
 UCSRC_N(USART_CLI_ID) = (1 << UCSZ1_N(USART_CLI_ID)) | (1 << UCSZ0_N(USART_CLI_ID)); // Set 1 stop bit, No parity bit.
// set baudrate
#undef BAUD
#define BAUD 57600
#include <util/setbaud.h>
 UBRRH_N(USART_CLI_ID) = UBRRH_VALUE;
 UBRRL_N(USART_CLI_ID) = UBRRL_VALUE;
#if USE_2X
 UCSRA_N(USART_CLI_ID) |= (1 << U2X_N(USART_CLI_ID));
#else
 UCSRA_N(USART_CLI_ID) &= ~(1 << U2X_N(USART_CLI_ID));
#endif
// enable RX
 UCSRB_N(USART_CLI_ID) |= (1 << RXEN_N(USART_CLI_ID));  // enable RX
 UCSRB_N(USART_CLI_ID) |= (1 << RXCIE_N(USART_CLI_ID)); // enable Interrupt
 while (UCSRA_N(USART_CLI_ID) & (1 << RXC_N(USART_CLI_ID))) UDR_N(USART_CLI_ID); // Flush RX buffer.
// enable TX
 UCSRB_N(USART_CLI_ID) |= (1 << TXEN_N(USART_CLI_ID)); // enable TX
}

void SerialCliPrint(const char * text)
{
 u8 len = strlen(text);

 if (len < SERIAL_CLI_TX_BUFFER_LENGHT) // check size
  {
   memcpy(SerialCliTXBuf[SerialCliTXBufWrite], text, len); // copy to TX buffer
   SerialCliTXBuf[SerialCliTXBufWrite][len] = SERIAL_CLI_EOL; // add end of line
   if(++SerialCliTXBufWrite >= SERIAL_CLI_TX_BUFFER_NUM) // Change buffer
    {
     SerialCliTXBufWrite = 0;
    }
   //UCSRB_N(USART_CLI_ID) |= (1 << UDRIE_N(USART_CLI_ID)); // enable Data Register Empty Interrupt
  }
 else
  {
   ERR("TEXTE TROP LONG");
  }
}

void checkSerialCliRXBuf()
{
 uint8_t cont = 1;
 do
  {
   if (SerialCliRXBufRead != SerialCliRXBufWrite) // there is some datas arrived
    {
     //computeSerialRXBuf(SerialRXBufRead); // Compute them !
     if(++SerialCliRXBufRead >= SERIAL_CLI_RX_BUFFER_NUM) SerialCliRXBufRead = 0; // Change buffer
    }
   else
    {
     cont = 0;
    }
  }
 while (cont);
}

void SerialCliSend()
{
 if (SerialCliTXBufWrite != SerialCliTXBufRead) // there is some datas to send
  {
   UCSRB_N(USART_CLI_ID) |= (1 << UDRIE_N(USART_CLI_ID)); // enable Data Register Empty Interrupt
  }
}

ISR(USART_RX_vect_N(USART_CLI_ID), ISR_NOBLOCK)
{
 static u8 SerialCliRXBufCount = 0;

 UCSRB_N(USART_CLI_ID) &= ~(1 << RXCIE_N(USART_CLI_ID)); // disable RX Interrupt
 u8 stat = UCSRA_N(USART_CLI_ID); // USART control and Status Register 0/1 A

 if (stat & ((1 << FE_N(USART_CLI_ID)) | (1 << DOR_N(USART_CLI_ID)) | (1 << UPE_N(USART_CLI_ID))))
  {
   // discard buffer and start fresh on any comms error
   SerialCliRXBufCount = 0; // reset counter
  }
 else
  {
   u8 data = UDR_N(USART_CLI_ID); // USART data register 0
   if (data != SERIAL_CLI_EOL)
    {
     SerialCliRXBuf[SerialCliRXBufWrite][SerialCliRXBufCount++] = data;
    }
   else
    {
     if(++SerialCliRXBufWrite >= SERIAL_CLI_RX_BUFFER_NUM)
      {
       SerialCliRXBufWrite = 0; // Change buffer
      }
     SerialCliRXBufCount = 0; // reset counter
    }
  }
 UCSRB_N(USART_CLI_ID) |= (1 << RXCIE_N(USART_CLI_ID)); // enable Interrupt
}

// USART0 Transmit Data Register Emtpy ISR (UDR was loaded in Shift Register)
ISR(USART_UDRE_vect_N(USART_CLI_ID))
{
 static u8 SerialCliTXBufCount = 0;

 if (SerialCliTXBufWrite != SerialCliTXBufRead) // there is some datas to send
  {
   char data = SerialCliTXBuf[SerialCliTXBufRead][SerialCliTXBufCount++];
   UDR_N(USART_CLI_ID) = data; // send data
   if (data == SERIAL_CLI_EOL)
    {
     if(++SerialCliTXBufRead >= SERIAL_CLI_TX_BUFFER_NUM) SerialCliTXBufRead = 0; // Change buffer
     SerialCliTXBufCount = 0; // reset counter
    }
  }
 else
  {
   UCSRB_N(USART_CLI_ID) &= ~(1 << UDRIE_N(USART_CLI_ID)); // Disable UDRE interrupt.
  }
}
