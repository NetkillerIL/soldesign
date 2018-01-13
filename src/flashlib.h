#ifndef FLASHLIB_H
#define FLASHLIB_H
#include <stdint.h>

/* pin definition*/
#define pinSS 10 // Chip select for SPI
#define pinMOSI 11 // MOSI pin for SPI
#define pinMISO 12 // MISO pin for SPI
#define pinSCK 13 // SysClock pin for SPI

/* fixes sized in memory*/
#define PAGES 2048
#define BYTES 65536
#define MEM_BLOCKS 7

#define BITSHIFT_HIGH 16
#define BITSHIFT_MID 8
#define BITSHIFT_LOW 0
/* OPCODES for flash memory */

/* functions declaration*/
void not_busy(void);
void read_manu_id(uint8_t * manu_info);
void deep_powerdown_on(void);
void deep_powerdown_off(void);
void read_page (uint32_t page, uint8_t * readBuf);
void write_array (uint32_t addr, uint8_t * w1, uint16_t arr_len);
void _write_byte (uint32_t addr, uint8_t w1);
uint8_t erase_block(uint32_t addr, uint16_t blocksize);
void _read_byte(uint32_t addr, uint8_t * b1);
void print_page (uint8_t * buf);
#endif
