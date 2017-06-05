#ifndef MKEBluetoothInterface_h
#define MKEBluetoothInterface_h

#include "Arduino.h"
#include "MKEM24M02.h"


//#define BLUETOOTH_DEBUG 0x22

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



#define DEVICE_CONF_CALLBACK 0x00
#define FILE_CALLBACK 0x20
#define DEVICE_INFO_CALLBACK 0x30
#define CURRENT_DATA_CALLBACK 0x40
#define CALIBRATION_CALLBACK 0x50
#define SAMPLER_CALLBACK 0x75
#define MOTOR_TEST_CALLBACK 0x60

typedef void(*commandCallback)(byte*,uint16_t);

class BluetoothInterface {

  public:
    BluetoothInterface();
    void config(uint32_t baudRate);
    uint8_t sendAllConfig();
    uint8_t sendFileNames();
    uint8_t sendDateTime();
    void setCallback(uint8_t callbacktype,commandCallback cc);
    void unsetCallback(uint8_t callbacktype);
    void receive();


  private:
    mEEPROM eeprom;
    uint8_t len;
    uint8_t msg[512];
    uint16_t cursor=0;
    bool deviceConnected;
    bool buffering;
    commandCallback fileCallback,devConfCallback,devInfoCallback,samplerCallback,calibrationCallback,currentDataCallback,motorTestCallback;


};

#endif
