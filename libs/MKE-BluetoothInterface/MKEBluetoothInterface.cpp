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
  fileCallback=NULL;
  devConfCallback=NULL;
  devInfoCallback = NULL;
  samplerCallback = NULL;
  calibrationCallback= NULL;
  currentDataCallback = NULL;
  motorTestCallback = NULL;
}

void BluetoothInterface::config(uint32_t baudRate) {

    Serial5.begin(baudRate);
}

void BluetoothInterface::setCallback(uint8_t callbackType,commandCallback cc){
  switch(callbackType){
    case FILE_CALLBACK:
      this->fileCallback = cc;
      break;
    case DEVICE_CONF_CALLBACK:
      this->devConfCallback = cc;
      break;
    case SAMPLER_CALLBACK:
      this->samplerCallback = cc;
      break;
    case CALIBRATION_CALLBACK:
      this->calibrationCallback = cc;
      break;
    case CURRENT_DATA_CALLBACK:
      this->currentDataCallback = cc;
      break;
    case DEVICE_INFO_CALLBACK:
      this->devInfoCallback = cc;
      break;
    case MOTOR_TEST_CALLBACK:
      this->motorTestCallback = cc;
      break;
    default:
      break;
  }
}

void BluetoothInterface::receive(){

  if(Serial5.available()){
    char c = Serial5.read();

    if(c == BUFFER_START_BYTE){
      buffering=true;
      cursor = 0;
      #ifdef BLUETOOTH_DEBUG
      Serial.println("Received Data to Bluetooth  :");
      Serial.print(c,DEC);
      Serial.print(" ");
      #endif
    }
    else if(c == BUFFER_END_BYTE){
      uint8_t *b;
      buffering=false;
      #ifdef BLUETOOTH_DEBUG
      Serial.print(c,DEC);
      Serial.println(" ");
      #endif
      switch(msg[0]){
        case FILE_CALLBACK:
          if(this->fileCallback!=NULL){
            this->fileCallback(msg,cursor);
          }
          break;
        case DEVICE_CONF_CALLBACK:
          if(this->devConfCallback!=NULL){
            this->devConfCallback(msg,cursor);
          }
          break;
        case SAMPLER_CALLBACK:
          if(this->samplerCallback!=NULL){
            this->samplerCallback(msg,cursor);
          }
          break;
        case CALIBRATION_CALLBACK:
          if(this->calibrationCallback!=NULL){
            this->calibrationCallback(msg,cursor);
          }
          break;
        case CURRENT_DATA_CALLBACK:
          if(this->currentDataCallback!=NULL){
            this->currentDataCallback(msg,cursor);
          }
          break;
        case DEVICE_INFO_CALLBACK:
          if(this->devInfoCallback!=NULL){
            this->devInfoCallback(msg,cursor);
          }
          break;
        case MOTOR_TEST_CALLBACK:
          if(this->motorTestCallback!=NULL){
            this->motorTestCallback(msg,cursor);
          }
          break;
        default:
          break;
      }
    }
    else if(buffering){
      #ifdef BLUETOOTH_DEBUG
      Serial.print(c,DEC);
      Serial.print(" ");
      #endif
      msg[cursor++]=c;
    }
  }
}
