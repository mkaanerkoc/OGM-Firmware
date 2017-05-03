/*

This is a library for the BH1750FVI Digital Light Sensor
breakout board.

The board uses I2C for communication. 2 pins are required to
interface to the device.


Written by Christopher Laws, March, 2013.

*/

#include "MKEM24M02.h"

EEPROM::EEPROM() {

}

void EEPROM::config(uint8_t enablePin_,uint8_t writeProtectPin_) {

  this->enablePin=enablePin;
  this->writeProtectPin=writeProtectPin_;
  pinMode(enablePin,OUTPUT);
  pinMode(writeProtectPin,OUTPUT);
  digitalWrite(enablePin,LOW);
  digitalWrite(writeProtectPin,LOW);
  Wire1.begin();

}

uint8_t EEPROM::writeSingleByte(uint32_t address,uint8_t val){
  //digitalWrite(writeProtectPin,LOW);
  int devAddr=0x50;
  uint8_t hiMemAddr  = (address & 0x030000)>>16;
  uint8_t midMemAddr = (address & 0x00ff00)>>8;
  uint8_t lowMemAddr = address & 0x0000ff;
  devAddr = devAddr | hiMemAddr;
  Wire1.beginTransmission(devAddr);
  Wire1.write(midMemAddr);
  Wire1.write(lowMemAddr);
  Wire1.write(val);
  uint8_t rr = Wire1.endTransmission();
//  digitalWrite(writeProtectPin,HIGH);
  return rr;
}

uint8_t EEPROM::readSingleByte(uint32_t address){
  int devAddr=0x50;
  uint8_t hiMemAddr  = (address & 0x030000)>>16;
  uint8_t midMemAddr = (address & 0x00ff00)>>8;
  uint8_t lowMemAddr = address & 0x0000ff;
  devAddr = devAddr | hiMemAddr;
  uint8_t retVal=0x33;
  Wire1.beginTransmission(devAddr);
  Wire1.write(midMemAddr);
  Wire1.write(lowMemAddr);
  Wire1.endTransmission();
  Wire1.requestFrom(devAddr,1);

  if(Wire1.available()){
    retVal = (uint8_t)Wire1.read();
  }

  Wire1.endTransmission();
  return retVal;
}

uint8_t EEPROM::writeConfigStruct(struct DeviceConfig cof){
  uint8_t *data = (uint8_t*)malloc(sizeof(cof));
  memcpy(data, &cof, sizeof(cof));
  int devAddr = 0x50;
  Wire1.beginTransmission(devAddr);
  Wire1.write(0x00);
  Wire1.write(0x00);
  for(unsigned int i = 0; i < sizeof(cof);i++){
    Wire1.write(data[i]);
  }
  uint8_t rr = Wire1.endTransmission();
  return rr;

}

uint8_t EEPROM::readConfigStruct(struct DeviceConfig *cof){
  uint8_t devAddr = 0x50;
  uint8_t index=0x00;
  //free(cof);
  uint8_t *str = (uint8_t*)malloc(sizeof(struct DeviceConfig));
  Wire1.beginTransmission(devAddr);
  Wire1.write(0x00);
  Wire1.write(0x00);
  Wire1.endTransmission();
  Wire1.requestFrom(devAddr,(uint8_t)sizeof(struct DeviceConfig));
  while(Wire1.available())    // slave may send less than requested
  {
   uint8_t c = Wire1.read(); // receive a byte as character
   str[index]=c;
   index++;
  }
  uint8_t rr = Wire1.endTransmission();
  memcpy(cof,str, sizeof(struct DeviceConfig));
  return rr;
}
