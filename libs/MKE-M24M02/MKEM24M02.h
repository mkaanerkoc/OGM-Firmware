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
#include "i2c_t3.h"

typedef struct DeviceConfig{
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

  uint16_t eventTimeOut;
  uint16_t timeBasedWaterSamplingTime;
  uint16_t eventBasedWaterSamplingTime;
  uint16_t timeBasedAcideSamplingTime;
  uint16_t eventBasedAcideSamplingTime;

  uint8_t readPeriod;
  uint16_t notchHeight;
}DeviceConfig;

typedef struct SamplerTimeConfig{
  uint8_t weekDay1;
  uint8_t hour1;
  uint8_t minute1;
  uint8_t weekDay2;
  uint8_t hour2;
  uint8_t minute2;

}SamplerTimeConfig;


typedef struct SamplerPercConfig{
  uint16_t periodicTotal;
  uint16_t periodicAcide;
  uint16_t periodicWater;
  uint16_t eventTotal;
  uint16_t eventAcide;
  uint16_t eventWater;
}SamplerPercConfig;

class mEEPROM {

  public:
    mEEPROM();
    void enable(uint8_t enablePin,uint8_t wpPin);
    void disable();
    void    scanBUS();
    uint8_t writeSingleByte(uint32_t address,uint8_t val);
    uint8_t readSingleByte(uint32_t address);
    uint8_t writeConfigStruct(struct DeviceConfig conf);
    uint8_t readConfigStruct(struct DeviceConfig *conf);

    uint8_t writeSamplerTimeStruct(struct SamplerTimeConfig conf);
    uint8_t readSamplerTimeStruct(struct SamplerTimeConfig *conf);

    uint8_t writeSamplerPercStruct(struct SamplerPercConfig conf);
    uint8_t readSamplerPercStruct(struct SamplerPercConfig *conf);


  private:

    uint8_t len;
    String msg;
    uint8_t enablePin;
    uint8_t writeProtectPin;
    void printKnownChips(byte address);
    bool deviceConnected;
    bool buffering;


};

#endif
