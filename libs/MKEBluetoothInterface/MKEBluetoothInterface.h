/*

This is a library for the BH1750FVI Digital Light Sensor
breakout board.

The board uses I2C for communication. 2 pins are required to
interface to the device.

Datasheet:
http://rohmfs.rohm.com/en/products/databook/datasheet/ic/sensor/light/bh1750fvi-e.pdf

Written by Christopher Laws, March, 2013.

*/

#ifndef MKEBluetoothInterface_h
#define MKEBluetoothInterface_h

#include "Arduino.h"
#include "MKEM24M02.h"


#define BUFFER_START_BYTE 0x2A
#define BUFFER_END_BYTE 0x26



#define SEND_CONFIG 0x01
#define SEND_FILE_NAMES 0x02
#define SEND_FILE 0x03
#define SEND_DATETIME 0x04


#define PH_CALIBRATION 0x10
#define OXY_CALIBRATION 0x11
#define TURBIDITY_CALIBRATION 0x12

#define READ_DISCRETE_INPUTS 0x30
#define WRITE_SINGLE_COIL 0x31
#define WRITE_MULTIPLE_COILS 0x32
#define READ_INPUT_REGISTER 0x33

#define HELLO 0x50
#define GOOD_BYE 0x51

#define READ_ALL_SENSORS 0x70

class BluetoothInterface {

  public:
    BluetoothInterface();
    void config(uint32_t baudRate);
    uint8_t sendAllConfig();
    uint8_t sendFileNames();
    uint8_t sendDateTime();
    void receive();


  private:
    EEPROM eeprom;
    uint8_t len;
    String msg;
    bool deviceConnected;
    bool buffering;


};

#endif
