#include "mcp4131.h"

mcp4131::mcp4131(uint8_t cs, uint8_t sdi, uint8_t sck)
{
    this->mSPISpec  = new SPISettings(SPI_SPEED_HZ, SPI_ENDIAN, SPI_MODE);
    this->mPinCS    = cs;
    this->mPinSDI   = sdi;
    this->mPinSCK   = sck;
}

mcp4131::~mcp4131()
{
    delete this->mSPISpec;
}

void mcp4131::writeValue(uint8_t val)
{
    SPI.beginTransaction(*this->mSPISpec);
    digitalWrite(this->mPinCS, LOW);
    delay(SPI_DELAY_MS);

    SPI.transfer(SPI_ADDR_WIPER0);
    delay(SPI_DELAY_MS);
    SPI.transfer(val);
    delay(SPI_DELAY_MS);

    digitalWrite(this->mPinCS, HIGH);
    SPI.endTransaction();
}