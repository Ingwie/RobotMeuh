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



#include "i2c.h"
#include <util/twi.h>


/*
 *        FCPU
 * --------------------  = SCL frequency
 * 16 + 2(TWBR) .4^TWPS
 */

void initI2C()
{
// active pullups
 set_output_off(I2cSclPin);
 set_output_off(I2cSdaPin);
// set I2C speed
 I2C_SPEED_888K();
}

void i2c_Wait()
{
// Poll TWI Interrupt flag.
 while(! (TWCR & (1<<TWINT)) );
}

u8 i2c_start(u8 address)
{
// transmit START condition
 TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
// i2c_Wait for end of transmission
 i2c_Wait();

// check if the start condition was successfully transmitted
 u8 twst = TW_STATUS & 0xF8;
 if(! ((twst == TW_START) || (twst == TW_REP_START)) )
  {
   i2c_stop();
   return 1;
  }

// load slave address into data register
 TWDR = address;
// start transmission of address
 TWCR = (1<<TWINT) | (1<<TWEN);
// i2c_Wait for end of transmission
 i2c_Wait();
// check if the device has acknowledged the READ / WRITE mode
 twst = TW_STATUS & 0xF8;
 if ( (twst == TW_MT_SLA_ACK) || (twst == TW_MR_SLA_ACK) ) return 0;
 else
  {
   i2c_stop();
   return 1;
  }
}

u8 i2c_write(u8 data)
{
// load data into data register
 TWDR = data;
// start transmission of data
 TWCR = (1<<TWINT) | (1<<TWEN);
// i2c_Wait for end of transmission
 i2c_Wait();

 if( (TWSR & 0xF8) != TW_MT_DATA_ACK )
  {
   return 1;
  }
 return 0;
}

u8 i2c_read_ack()
{
// start TWI module and acknowledge data after reception
 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
// i2c_Wait for end of transmission
 i2c_Wait();
// return received data from TWDR
 return TWDR;
}

u8 i2c_read_nack()
{
// start receiving without acknowledging reception
 TWCR = (1<<TWINT) | (1<<TWEN);
// i2c_Wait for end of transmission
 i2c_Wait();
// return received data from TWDR
 return TWDR;
}

void i2c_stop()
{
// transmit STOP condition
 TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
 while (TWCR & (1<<TWSTO));
}

void i2c_writeAndActiveISR(u8 data)
{
// load data into data register
 TWDR = data;
// start transmission of data with isr activated
 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
}


/*
ISR(TWI_vect)
{
  if (--i2c_buffer_size) {     // If there is another byte to write
    TWDR = *++i2c_buffer_data; // Increase pointer and write
  // start transmission of data
  TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWIE);
  } else {
  // transmit STOP condition this reset TWI interrupts
  TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
  //while SIMU_UNLOCK_MACRO_FALSE(TWCR & (1<<TWSTO)); // don't wait completion
  }
}
*/

u8 i2c_transmit(u8 address, u8* data, u16 length)
{
 if (i2c_start(address | I2C_WRITE)) return 1;

 for (u16 i = 0; i < length; i++)
  {
   if (i2c_write(data[i])) return 1;
  }

 i2c_stop();

 return 0;
}

u8 i2c_receive(u8 address, u8* data, u16 length)
{
 if (i2c_start(address | I2C_READ)) return 1;

 for (u16 i = 0; i < (length-1); i++)
  {
   data[i] = i2c_read_ack();
  }
 data[(length-1)] = i2c_read_nack();

 i2c_stop();

 return 0;
}

u8 i2c_writeReg(u8 devaddr, u8 regaddr, u8* data, u16 length)
{
 if (i2c_start(devaddr | I2C_WRITE)) return 1;

 i2c_write(regaddr);

 for (u16 i = 0; i < length; i++)
  {
   if (i2c_write(data[i])) return 1;
  }

 i2c_stop();

 return 0;
}

u8 i2c_writeRegByte(u8 devaddr, u8 regaddr, u8 data)
{
 if (i2c_start(devaddr | I2C_WRITE)) return 1;

 i2c_write(regaddr);

 if (i2c_write(data)) return 1;

 i2c_stop();

 return 0;
}

u8 i2c_readReg(u8 devaddr, u8 regaddr, u8* data, u16 length)
{
 if (i2c_start(devaddr | I2C_WRITE)) return 1;

 i2c_write(regaddr);

 if (i2c_start(devaddr | I2C_READ)) return 1;

 for (u16 i = 0; i < (length-1); i++)
  {
   data[i] = i2c_read_ack();
  }
 data[(length-1)] = i2c_read_nack();

 i2c_stop();

 return 0;
}
