 uint8_t indicator = 0;
      Serial.println("Initialize write buffer...");
      for(int i=0; i < 256; i++) {
        writeBuf2[i] = '\0';
      }
      erase_block(0x000000, 512);
      Serial.println("Test 1 - empty memory...");
      for ( int i = 1; i < 2046; i++) {
        read_page(i,1);
        for (int j = 0; j < 255; j++) {
          if (readBuf[j] != 0xff) {
            indicator = 1;
          }
        }
      }

      Serial.println("finished testing");
      Serial.println("Test 2 - writing...");
      for ( int i = 0; i < 2046; i++) {
        write_array(i * 0x000100, writeBuf);
      }
      Serial.println("Test 2 - reading...");

      for ( int i = 1; i < 2045; i++) {
        read_page(i,1);
          if (readBuf[0] != 0x68) {
            indicator = 1;
          }
          if (readBuf[1] != 0x65) {
            indicator = 1;
          }
          if (readBuf[2] != 0x6c) {
            indicator = 1;
          }
          if (readBuf[3] != 0x6c) {
            indicator = 1;
          }
          if (readBuf[4] != 0x6f) {
            indicator = 1;
          }
      }
      if (indicator == 0)
        Serial.println("Success");
      else
        Serial.println("Failure!");


      //Serial.println("erasing memory...");
      //erase_block(0x000000, 512);
      Serial.println("Done testing.");

      break;

      /*test 3 and 4*/

