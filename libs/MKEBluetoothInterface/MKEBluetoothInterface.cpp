/*

This is a library for the BH1750FVI Digital Light Sensor
breakout board.

The board uses I2C for communication. 2 pins are required to
interface to the device.


Written by Christopher Laws, March, 2013.

*/

#include "MKEBluetoothInterface.h"

BluetoothInterface::BluetoothInterface() {
  deviceConnected=false;
  buffering=false;
}

void BluetoothInterface::config(uint32_t baudRate) {

    Serial5.begin(baudRate);
}

void BluetoothInterface::receive(){

  if(Serial5.available()){
    char c = Serial5.read();
    if(c == BUFFER_START_BYTE){
      buffering=true;
      msg = "";
    }
    else if(c == BUFFER_END_BYTE){
      uint8_t *b;
      buffering=false;
      switch(msg[0]){
        case SEND_CONFIG:
          eeprom.config(36,39);
          Serial.print("Send Config");
          struct DeviceConfig devConf;
          eeprom.readConfigStruct(&devConf);
          b = (uint8_t*)malloc(sizeof(struct DeviceConfig)+2);
          b[0]='*';
          b[sizeof(b)-1]='&';
          memcpy(b+1,&devConf,sizeof(struct DeviceConfig));
          for(int j = 0 ; j < sizeof(b);j++){
            Serial5.write(b[j]);
          }
          break;
        case SEND_FILE_NAMES:
          break;
        case SEND_FILE:
          break;
        case SEND_DATETIME:
          break;
        case PH_CALIBRATION:
          break;
        case OXY_CALIBRATION:
          break;
        case TURBIDITY_CALIBRATION:
          break;
        case READ_DISCRETE_INPUTS:
          break;
        case WRITE_SINGLE_COIL:
          break;
        case WRITE_MULTIPLE_COILS:
          break;
        case READ_INPUT_REGISTER:
          break;
        case HELLO:
          break;
        case GOOD_BYE:
          break;
        case READ_ALL_SENSORS:
          Serial.write(0x70);
          break;
        default:
          break;
      }
    }
    else if(buffering){
      msg += c;
    }
  }
}
