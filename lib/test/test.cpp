#include <flashlib.h>
#include <test.h>
#include <Arduino.h>
#include <SPI.h>

void Test::init(void) {

}

void Test::ltos (uint32_t data, int cnt) {
  writeBufs[cnt*4] = (uint8_t)(data >> 24);
  writeBufs[cnt*4 + 1] = (uint8_t)(data >> 16);
  writeBufs[cnt*4 + 2] = (uint8_t)(data >> 8);
  writeBufs[cnt*4 + 3] = (uint8_t)(data >> 0);
}  

void Test::test_mem1 (void) {
  Serial.println("erasing flash");
  erase_block(0x000000, 512);

  Serial.println("prepairing random array");
  for (int i = 0; i < PAYL_IN_BLCK; i++) {
    for (int j = 0; j < PAYL_SIZE; j++) {
      ltos(random(0xfffffff), j);
    }
    write_array(0x000090*i, writeBufs, 144);
  }

  for (int i = 1; i < PAGES_IN_BLCK; i++) {
      read_page(i, readBuf);
      print_page(readBuf);
    }
}
