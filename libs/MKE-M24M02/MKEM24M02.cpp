/*

This is a library for the BH1750FVI Digital Light Sensor
breakout board.

The board uses I2C for communication. 2 pins are required to
interface to the device.


Written by Christopher Laws, March, 2013.

*/

#include "MKEM24M02.h"

mEEPROM::mEEPROM() {

}

void mEEPROM::enable(uint8_t enablePin_,uint8_t writeProtectPin_) {

  this->enablePin=enablePin;
  this->writeProtectPin=writeProtectPin_;
  /*pinMode(enablePin,OUTPUT);
  pinMode(writeProtectPin,OUTPUT);
  digitalWrite(enablePin,LOW);
  digitalWrite(writeProtectPin,LOW);*/
  Wire1.begin();

}
void mEEPROM::disable() {

  pinMode(this->enablePin,INPUT);
  pinMode(this->writeProtectPin,INPUT);
  Wire1.endTransmission();

}

uint8_t mEEPROM::writeSingleByte(uint32_t address,uint8_t val){
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

uint8_t mEEPROM::readSingleByte(uint32_t address){
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

uint8_t mEEPROM::writeConfigStruct(struct DeviceConfig cof){
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
  free(data);
  return rr;

}

uint8_t mEEPROM::readConfigStruct(struct DeviceConfig *cof){
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
  free(str);
  return rr;
}
uint8_t mEEPROM::writeSamplerTimeStruct(struct SamplerTimeConfig cof){
  uint8_t *data = (uint8_t*)malloc(sizeof(struct SamplerTimeConfig));
  memcpy(data, &cof, sizeof(cof));
  int devAddr = 0x50;
  Wire1.beginTransmission(devAddr);
  Wire1.write(0x01);
  Wire1.write(0x00);
  for(unsigned int i = 0; i < sizeof(struct SamplerTimeConfig);i++){
    Wire1.write(data[i]);
  }
  uint8_t rr = Wire1.endTransmission();
  free(data);
  return rr;

}

uint8_t mEEPROM::readSamplerTimeStruct(struct SamplerTimeConfig *cof){

  uint8_t devAddr = 0x50;
  uint8_t index=0x00;
  //free(cof);
  uint8_t *str = (uint8_t*)malloc(sizeof(struct SamplerTimeConfig));
  Wire1.beginTransmission(devAddr);
  Wire1.write(0x01);
  Wire1.write(0x00);
  Wire1.endTransmission();
  Wire1.requestFrom(devAddr,(uint8_t)sizeof(struct SamplerTimeConfig));
  while(Wire1.available())    // slave may send less than requested
  {
   uint8_t c = Wire1.read(); // receive a byte as character
   str[index]=c;
   index++;
  }
  uint8_t rr = Wire1.endTransmission();
  memcpy(cof,str, sizeof(struct SamplerTimeConfig));
  free(str);
  return rr;
}


uint8_t mEEPROM::writeSamplerPercStruct(struct SamplerPercConfig cof){
  uint8_t *data = (uint8_t*)malloc(sizeof(struct SamplerPercConfig));
  memcpy(data, &cof, sizeof(cof));
  int devAddr = 0x50;
  Wire1.beginTransmission(devAddr);
  Wire1.write(0x02);
  Wire1.write(0x00);
  for(unsigned int i = 0; i < sizeof(struct SamplerPercConfig);i++){
    Wire1.write(data[i]);
  }
  uint8_t rr = Wire1.endTransmission();
  free(data);
  return rr;
}
uint8_t mEEPROM::readSamplerPercStruct(struct SamplerPercConfig *cof){
  uint8_t devAddr = 0x50;
  uint8_t index=0x00;
  //free(cof);
  uint8_t *str = (uint8_t*)malloc(sizeof(struct SamplerPercConfig));
  Wire1.beginTransmission(devAddr);
  Wire1.write(0x02);
  Wire1.write(0x00);
  Wire1.endTransmission();
  Wire1.requestFrom(devAddr,(uint8_t)sizeof(struct SamplerPercConfig));
  while(Wire1.available())    // slave may send less than requested
  {
   uint8_t c = Wire1.read(); // receive a byte as character
   str[index]=c;
   index++;
  }
  uint8_t rr = Wire1.endTransmission();
  memcpy(cof,str, sizeof(struct SamplerPercConfig));
  free(str);
  return rr;
}

void mEEPROM::scanBUS(void){
  byte error, address;
  int nDevices;

  Serial.println(F("Scanning..."));

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0) {
      Serial.print(F("Device found at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address,HEX);
      Serial.print("  (");
      printKnownChips(address);
      Serial.println(")");

      nDevices++;
    } else if (error==4) {
      Serial.print(F("Unknown error at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println(F("No I2C devices found\n"));
  } else {
    Serial.println(F("done\n"));
  }
}

void mEEPROM::printKnownChips(byte address)
{
  // Is this list missing part numbers for chips you use?
  // Please suggest additions here:
  // https://github.com/PaulStoffregen/Wire1/issues/new
  switch (address) {
    case 0x00: Serial.print(F("AS3935")); break;
    case 0x01: Serial.print(F("AS3935")); break;
    case 0x02: Serial.print(F("AS3935")); break;
    case 0x03: Serial.print(F("AS3935")); break;
    case 0x0A: Serial.print(F("SGTL5000")); break; // MCLK required
    case 0x0B: Serial.print(F("SMBusBattery?")); break;
    case 0x0C: Serial.print(F("AK8963")); break;
    case 0x10: Serial.print(F("CS4272")); break;
    case 0x11: Serial.print(F("Si4713")); break;
    case 0x13: Serial.print(F("VCNL4000,AK4558")); break;
    case 0x18: Serial.print(F("LIS331DLH")); break;
    case 0x19: Serial.print(F("LSM303,LIS331DLH")); break;
    case 0x1A: Serial.print(F("WM8731")); break;
    case 0x1C: Serial.print(F("LIS3MDL")); break;
    case 0x1D: Serial.print(F("LSM303D,LSM9DS0,ADXL345,MMA7455L,LSM9DS1,LIS3DSH")); break;
    case 0x1E: Serial.print(F("LSM303D,HMC5883L,FXOS8700,LIS3DSH")); break;
    case 0x20: Serial.print(F("MCP23017,MCP23008,FXAS21002")); break;
    case 0x21: Serial.print(F("MCP23017,MCP23008")); break;
    case 0x22: Serial.print(F("MCP23017,MCP23008")); break;
    case 0x23: Serial.print(F("MCP23017,MCP23008")); break;
    case 0x24: Serial.print(F("MCP23017,MCP23008")); break;
    case 0x25: Serial.print(F("MCP23017,MCP23008")); break;
    case 0x26: Serial.print(F("MCP23017,MCP23008")); break;
    case 0x27: Serial.print(F("MCP23017,MCP23008,LCD16x2,DigoleDisplay")); break;
    case 0x28: Serial.print(F("BNO055,EM7180,CAP1188")); break;
    case 0x29: Serial.print(F("TSL2561,VL6180,TSL2561,TSL2591,BNO055,CAP1188")); break;
    case 0x2A: Serial.print(F("SGTL5000,CAP1188")); break;
    case 0x2B: Serial.print(F("CAP1188")); break;
    case 0x2C: Serial.print(F("MCP44XX ePot")); break;
    case 0x2D: Serial.print(F("MCP44XX ePot")); break;
    case 0x2E: Serial.print(F("MCP44XX ePot")); break;
    case 0x2F: Serial.print(F("MCP44XX ePot")); break;
    case 0x38: Serial.print(F("RA8875,FT6206")); break;
    case 0x39: Serial.print(F("TSL2561")); break;
    case 0x3C: Serial.print(F("SSD1306,DigisparkOLED")); break;
    case 0x3D: Serial.print(F("SSD1306")); break;
    case 0x40: Serial.print(F("PCA9685,Si7021")); break;
    case 0x41: Serial.print(F("STMPE610,PCA9685")); break;
    case 0x42: Serial.print(F("PCA9685")); break;
    case 0x43: Serial.print(F("PCA9685")); break;
    case 0x44: Serial.print(F("PCA9685")); break;
    case 0x45: Serial.print(F("PCA9685")); break;
    case 0x46: Serial.print(F("PCA9685")); break;
    case 0x47: Serial.print(F("PCA9685")); break;
    case 0x48: Serial.print(F("ADS1115,PN532,TMP102,PCF8591")); break;
    case 0x49: Serial.print(F("ADS1115,TSL2561,PCF8591")); break;
    case 0x4A: Serial.print(F("ADS1115")); break;
    case 0x4B: Serial.print(F("ADS1115,TMP102")); break;
    case 0x50: Serial.print(F("EEPROM")); break;
    case 0x51: Serial.print(F("EEPROM")); break;
    case 0x52: Serial.print(F("Nunchuck,EEPROM")); break;
    case 0x53: Serial.print(F("ADXL345,EEPROM")); break;
    case 0x54: Serial.print(F("EEPROM")); break;
    case 0x55: Serial.print(F("EEPROM")); break;
    case 0x56: Serial.print(F("EEPROM")); break;
    case 0x57: Serial.print(F("EEPROM")); break;
    case 0x58: Serial.print(F("TPA2016,MAX21100")); break;
    case 0x5A: Serial.print(F("MPR121")); break;
    case 0x60: Serial.print(F("MPL3115,MCP4725,MCP4728,TEA5767,Si5351")); break;
    case 0x61: Serial.print(F("MCP4725")); break;
    case 0x62: Serial.print(F("LidarLite,MCP4725")); break;
    case 0x63: Serial.print(F("MCP4725")); break;
    case 0x68: Serial.print(F("DS1307,DS3231,MPU6050,MPU9050,MPU9250,ITG3200,ITG3701,LSM9DS0,L3G4200D")); break;
    case 0x69: Serial.print(F("MPU6050,MPU9050,MPU9250,ITG3701,L3G4200D")); break;
    case 0x6A: Serial.print(F("LSM9DS1")); break;
    case 0x6B: Serial.print(F("LSM9DS0")); break;
    case 0x70: Serial.print(F("AdafruitLED")); break;
    case 0x71: Serial.print(F("SFE7SEG,AdafruitLED")); break;
    case 0x72: Serial.print(F("AdafruitLED")); break;
    case 0x73: Serial.print(F("AdafruitLED")); break;
    case 0x76: Serial.print(F("MS5607,MS5611,MS5637,BMP280")); break;
    case 0x77: Serial.print(F("BMP085,BMA180,BMP280,MS5611")); break;
    default: Serial.print(F("unknown chip"));
  }
}
