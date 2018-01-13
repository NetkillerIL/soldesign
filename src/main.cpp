#include <Arduino.h>
#include <SPI.h>
#include "flashlib.h"

/* pin definition*/
#define pinSS 10 // Chip select for SPI
#define pinMOSI 11 // MOSI pin for SPI
#define pinMISO 12 // MISO pin for SPI
#define pinSCK 13 // SysClock pin for SPI

long writeBuf2[1]; // test string
uint8_t writeBufs[144];
uint8_t lass[4];
uint8_t readBuf[256];

void setup() {
    Serial.begin(9600);
    SPI.begin();
    pinMode(pinMOSI, OUTPUT);
    pinMode(pinMISO, INPUT);
    Serial.write("Enter a functionality: ");
}

void ltos(uint32_t data, int cnt) {
  writeBufs[cnt*4] = (uint8_t)(data >> 24);
  writeBufs[cnt*4 + 1] = (uint8_t)(data >> 16);
  writeBufs[cnt*4 + 2] = (uint8_t)(data >> 8);
  writeBufs[cnt*4 + 3] = (uint8_t)(data >> 0);
}

void loop() {
  switch (Serial.read()) {
    case 't':
    Serial.println("erasing flash");
    erase_block(0x000000, 512);
    delay(1000);
    Serial.println("prepairing random array");
    for (int i = 0; i < 28; i++) {
      for (int j = 0; j < 36; j++) {
        ltos(random(0xfffffff), j);
      }
      write_array(0x000090*i, writeBufs, 144);


    }

    for (int i = 1; i < 17; i++) {
      read_page(i, readBuf);
      print_page(readBuf);
    }



    /*
    write_array(0x000000, writeBufs);
    Serial.println("Page number 1");
    read_page(1, readBuf);
    print_page(readBuf);
    Serial.println("Page number 2");
    read_page(2, readBuf);
    print_page(readBuf);
    Serial.println("done.");
    */
    break;
    }

}
