/*

This is a library for the BH1750FVI Digital Light Sensor
breakout board.

The board uses I2C for communication. 2 pins are required to
interface to the device.

Datasheet:
http://rohmfs.rohm.com/en/products/databook/datasheet/ic/sensor/light/bh1750fvi-e.pdf

Written by Christopher Laws, March, 2013.

*/

#ifndef MKEM24M02_h
#define MKEM24M02_h

#include "Arduino.h"
#include <i2c_t3.h>

struct DeviceConfig{
  uint8_t deviceID;
  uint8_t fieldID;
  float phLowThr;
  float phHighThr;
  float conductivityLowThr;
  float conductivityHighThr;
  float oxygenLowThr;
  float oxygenHighThr;
  float turbidityLowThr;
  float turbidityHighThr;
  float rainHighThr;
  uint16_t timeBasedWaterSamplingTime;
  uint16_t eventBasedWaterSamplingTime;
  uint16_t timeBasedAcideSamplingTime;
  uint16_t eventBasedAcideSamplingTime;
  uint8_t readPeriod;
  uint16_t notchHeight;
};

class EEPROM {

  public:
    EEPROM();
    void config(uint8_t enablePin,uint8_t wpPin);
    uint8_t writeSingleByte(uint32_t address,uint8_t val);
    uint8_t readSingleByte(uint32_t address);
    uint8_t writeConfigStruct(struct DeviceConfig conf);
    uint8_t readConfigStruct(struct DeviceConfig *conf);


  private:

    uint8_t len;
    String msg;
    uint8_t enablePin;
    uint8_t writeProtectPin;
    bool deviceConnected;
    bool buffering;


};

#endif
