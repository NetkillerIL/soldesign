#ifndef TEST_H
#define TEST_H
#include <stdint.h>

#define PAYL_SIZE 36
#define PAYL_IN_BLCK 28
#define PAGES_IN_BLCK 17

class Test {  
  private:
    uint8_t writeBufs[144];
    void ltos(uint32_t data, int cnt);

  public:
    uint8_t readBuf[256];

    void init(void);
    void test_mem1(void);
};

#endif