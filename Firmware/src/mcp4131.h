// https://www.mouser.co.uk/datasheet/2/268/22060a-53835.pdf

#ifndef MCP4131_H
#define MCP4131_H

#define SPI_ADDR_WIPER0 0x00
#define SPI_SPEED_HZ    10000000
#define SPI_ENDIAN      MSBFIRST
#define SPI_MODE        SPI_MODE0

#include <SPI.h>
#include "consts.h"

class mcp4131 {
public:
    mcp4131(uint8_t cs, uint8_t sdi, uint8_t sck);
    ~mcp4131();
    void writeValue(uint8_t val);

private:
    SPISettings* mSPISpec;
    uint8_t mPinCS;
    uint8_t mPinSDI;
    uint8_t mPinSCK;
};

#endif