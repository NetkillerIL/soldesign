#include <SPI.h>
//#include <stdint.h>
#include <flashlib.h>

const uint8_t READ_BYTE_FLASH = 0x03;
const uint8_t WRITE_ENABLE = 0x06;
const uint8_t WRITE_BYTE_FLASH = 0x02;
const uint8_t BLOCK_ERASE_8 = 0x20;
const uint8_t BLOCK_ERASE_32 = 0x52;
const uint8_t BLOCK_ERASE_64 = 0xD8;
const uint8_t CHIP_ERASE = 0xC7;
const uint8_t DEEP_POWERDOWN = 0xB9;
const uint8_t RESUME_FROM_POWERDOWN = 0xAB;
const uint8_t READ_MANU_ID = 0x9F;
const uint8_t STATUS_REG_1 = 0x05;
/* global variables*/
uint8_t info[3]; //holdes info of manufacturer
uint8_t b1; //char result - debugging purposes
uint8_t w1, w2, w3; //char - debugging purposes
char writeBuf[6] = "hello"; // test string
//long writeBuf2[144]; // test string
//long writeBuf2[144]; // test string



/*
 *  The function preforms "Read Status Register Byte 1"
 *  and checks if the flash is busy W/R, it waits until
 *  it is not busy anymore
*/

void not_busy(void) {
  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);
  SPI.transfer(STATUS_REG_1);
  while (SPI.transfer(0) & 1);
  digitalWrite(pinSS, HIGH);
}



/*   general functions */
void read_manu_id(uint8_t * manu_info) {

  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(READ_MANU_ID);

  *(manu_info+1) = SPI.transfer(0);
  *(manu_info+2) = SPI.transfer(0);
  *(manu_info+3) = SPI.transfer(0);

  digitalWrite(pinSS, HIGH);
  not_busy();
}

void deep_powerdown_on () {
  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(DEEP_POWERDOWN);

  digitalWrite(pinSS, HIGH);
  //not_busy();

}



void deep_powerdown_off () {
  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(RESUME_FROM_POWERDOWN);

  digitalWrite(pinSS, HIGH);
  not_busy();

}


/* read functions*/
void _read_byte (uint32_t addr,uint8_t * b1) {
  not_busy();

  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(READ_BYTE_FLASH);
  SPI.transfer((uint8_t)(addr >> BITSHIFT_HIGH));
  SPI.transfer((uint8_t)(addr >> BITSHIFT_MID));
  SPI.transfer((uint8_t)(addr >> BITSHIFT_LOW));

  *b1 = SPI.transfer(0);

  digitalWrite(pinSS, HIGH);

}
void read_page (uint32_t page, uint8_t * readBuf) {
  uint32_t pageAddr = ( page -1 ) << 8;

  for (int cnt = 0; cnt < 256; cnt++) {
    _read_byte(pageAddr+cnt, &b1);
    readBuf[cnt] = b1;
  }

}
/*
***** (NOT TO USE - USE write_array(...) instead )*****

  write a single byte - a private function used
   to write bytes safely without "wrong byte-alignment"
   which causes bits to "wrap around a byte in memory"

***** (NOT TO USE - USE write_array(...) instead )*****
*/
void _write_byte (uint32_t addr, uint8_t w1) {

  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(WRITE_ENABLE);

  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(WRITE_BYTE_FLASH);

  SPI.transfer((uint8_t)(addr >> BITSHIFT_HIGH));
  SPI.transfer((uint8_t)(addr >> BITSHIFT_MID));
  SPI.transfer((uint8_t)(addr >> BITSHIFT_LOW));

  SPI.transfer(w1);

  digitalWrite(pinSS, HIGH);
  not_busy();

}

void write_array (uint32_t addr, uint8_t * w1, uint16_t arr_len) {
  if (sizeof(w1) > BYTES) {
    Serial.println("Not enough space in flash! please write less than 65536 bytes");
  }
  else {
    for(int j =  0; j < arr_len; j++, addr++, w1++ ) {
      _write_byte(addr, *w1);
    }
  }
}
/*
 * erase_block
 * uint32_t addr - address in memory to start erasing from
 * uint8_t block_size - block size to erase - 8, 32, 64, 512 - whole chip
 * return the erased block size
*/
uint8_t erase_block (uint32_t addr, uint16_t block_size ) {
    uint8_t blockSizeOp;
    switch (block_size) {
	    // must change to 4 - 8 is wrong 
        case 8 :
            blockSizeOp = BLOCK_ERASE_8;
            break;
        case 32 :
            blockSizeOp = BLOCK_ERASE_32;
            break;
        case 64 :
            blockSizeOp = BLOCK_ERASE_64;
            break;
        case 512:
            blockSizeOp = CHIP_ERASE;
            break;
    }

  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(WRITE_ENABLE);

  digitalWrite(pinSS, HIGH);
  digitalWrite(pinSS, LOW);

  SPI.transfer(blockSizeOp);
  if (block_size != 512) {
    SPI.transfer((uint8_t)(addr >> BITSHIFT_HIGH));
    SPI.transfer((uint8_t)(addr >> BITSHIFT_MID));
    SPI.transfer((uint8_t)(addr >> BITSHIFT_LOW));
  }
  digitalWrite(pinSS, HIGH);
  not_busy();

  return block_size;
}

/* read a single byte for memory - deprecated
   for debugging purposes*/


/*prints the page content direct from the corresponding buffer*/
void print_page (uint8_t * buf) {
  for (int cnt = 0; cnt < 256; cnt++) {
    if ( !(cnt % 16) ) {
      Serial.println(" ");
    }
    else {
      if (!(cnt % 4)) {
        Serial.print(" ");
      }
    }
    Serial.print(buf[cnt], HEX);
  }
  Serial.println(" ");
  Serial.println("done!");
}
