/*

This is a library for the BH1750FVI Digital Light Sensor
breakout board.

The board uses I2C for communication. 2 pins are required to
interface to the device.


Written by Christopher Laws, March, 2013.

*/

#include "MKEModbusMaster.h"

ModbusMaster::ModbusMaster() {

}

void ModbusMaster::config(uint16_t baudRate) {
  //this->_port=port;
  Serial1.begin(baudRate,SERIAL_8N1);

}

uint8_t ModbusMaster::writeSingleRegister(uint8_t slaveID,uint16_t registerAddress,uint16_t registerValue,uint16_t timeOut){
  uint8_t _cnt = 0;
  free(msg);
  msg = (uint8_t*)malloc(sizeof(uint8_t)*8);
  msg[0]=slaveID;
  msg[1]=0x06;
  msg[2]=0xff&(registerAddress>>8);
  msg[3]=(registerAddress & 0xff);
  msg[4]=0xff&(registerValue>>8);
  msg[5]=(registerValue & 0xff);
  uint16_t crc = calculateCRC(slaveID,(msg+1),5);
  msg[6]=crc>>8;
  msg[7]=(crc&0xff);
  _timer=millis();
  #ifdef _MODBUS_DEBUG_
  Serial.println("Send Data :");
  #endif
  for(int i = 0 ; i < 8 ; i++){
    Serial1.write(msg[i]);
    #ifdef _MODBUS_DEBUG_
    Serial.print(msg[i],DEC);
    Serial.print(" ");
    #endif
    msg[i]=0;
  }
  #ifdef _MODBUS_DEBUG_
  Serial.println("");
  #endif
  len=8;
  uint16_t rcv_crc=0x0000;
  #ifdef _MODBUS_DEBUG_
  Serial.println("Received Data : ");
  #endif
  while(millis()<_timer+timeOut){
    if(Serial1.available()>0){
      char c = Serial1.available();
      msg[_cnt]=(uint8_t)Serial1.read();
      #ifdef _MODBUS_DEBUG_
      Serial.print(msg[_cnt],DEC);
      Serial.print(" ");
      #endif
      _cnt++;
      if(_cnt==len)break;
    }
  }
  #ifdef _MODBUS_DEBUG_
  Serial.println(" ");
  #endif
  crc = calculateCRC(slaveID,(msg+1),5);
  rcv_crc=(msg[len-2]<<8)|msg[len-1];
  if(crc==rcv_crc){
    return 1;
  }
  else{
    return 0;
  }
  //Serial1.flush();
}
uint16_t* ModbusMaster::readInputRegisters(uint8_t slaveID,uint16_t registerAddress,uint16_t registerCount,uint16_t timeOut){

  free(msg);
  free(output);
  uint8_t _cnt=0;
  output = (uint16_t*)malloc(sizeof(uint16_t)*registerCount);
  for(int i = 0 ; i < registerCount;i++){
    output[i]=0xffff;
  }
  msg = (uint8_t*)malloc(sizeof(uint8_t)*8);
  if(!msg){
    #ifdef _MODBUS_DEBUG_
    Serial.println("An error occured..");
    #endif
    return output;
  }
  msg[0]=slaveID;
  msg[1]=0x04;
  msg[2]=0xff&(registerAddress>>8);
  msg[3]=(registerAddress & 0xff);
  msg[4]=0xff&(registerCount>>8);
  msg[5]=(registerCount & 0xff);
  uint16_t crc = calculateCRC(slaveID,(msg+1),5);
  msg[6]=crc>>8;
  msg[7]=(crc&0xff);
  len = 5+registerCount*2;
  #ifdef _MODBUS_DEBUG_
  Serial.println("Send Data :");
  #endif
  _timer=millis();
  for(int i = 0 ; i < 8 ; i++){
    Serial1.write(msg[i]);
    #ifdef _MODBUS_DEBUG_
    Serial.print(msg[i],DEC);
    Serial.print(" ");
    #endif
    msg[i]=0;
  }
  #ifdef _MODBUS_DEBUG_
  Serial.println("");
  #endif
  //Serial1.flush();
  free(msg);
  msg=(uint8_t*)malloc(sizeof(uint8_t)*len);
  #ifdef _MODBUS_DEBUG_
  Serial.println("Received Data :");
  #endif
  while(millis()<_timer+timeOut){
    if(Serial1.available()>0){
      char c = Serial1.available();
      msg[_cnt]=(uint8_t)Serial1.read();
      #ifdef _MODBUS_DEBUG_
      Serial.print(msg[_cnt],DEC);
      Serial.print(" ");
      #endif
      _cnt++;
      if(_cnt==len)break;
    }
  }
  #ifdef _MODBUS_DEBUG_
  Serial.println("");
  #endif
  //Serial1.flush();

  crc = calculateCRC(slaveID,(msg+1),len-3);
  if(crc==((msg[len-2]<<8)|msg[len-1])){
    //CRC IS CORRECT
    uint8_t byteCount = msg[2];
    if(byteCount==(registerCount*2)){
      for(int i = 0 ; i < registerCount;i++){
        output[i]=(msg[i*2+3]<<8)|msg[i*2+4];
      }
      return output;
    }
  }
  else{
    return 0;
  }
}

uint16_t* ModbusMaster::readHoldingRegisters(uint8_t slaveID,uint16_t registerAddress,uint16_t registerCount,uint16_t timeOut){

  free(msg);
  free(output);
  uint8_t _cnt=0;
  output = (uint16_t*)malloc(sizeof(uint16_t)*registerCount);
  for(int i = 0 ; i < registerCount;i++){
    output[i]=0xffff;
  }
  msg = (uint8_t*)malloc(sizeof(uint8_t)*8);
  if(!msg){
    #ifdef _MODBUS_DEBUG_
    Serial.println("An error occured..");
    #endif
    return output;
  }
  msg[0]=slaveID;
  msg[1]=0x03;
  msg[2]=0xff&(registerAddress>>8);
  msg[3]=(registerAddress & 0xff);
  msg[4]=0xff&(registerCount>>8);
  msg[5]=(registerCount & 0xff);
  uint16_t crc = calculateCRC(slaveID,(msg+1),5);
  msg[6]=crc>>8;
  msg[7]=(crc&0xff);
  len = 5+registerCount*2;

  _timer=millis();
  #ifdef _MODBUS_DEBUG_
  Serial.println("Send data :");
  #endif
  for(int i = 0 ; i < 8 ; i++){
    Serial1.write(msg[i]);
    #ifdef _MODBUS_DEBUG_
    Serial.print(msg[i],DEC);
    Serial.print(" ");
    #endif
    msg[i]=0;
  }
  #ifdef _MODBUS_DEBUG_
  Serial.println("");
  #endif
  //Serial1.flush();
  free(msg);
  msg=(uint8_t*)malloc(sizeof(uint8_t)*len);
  #ifdef _MODBUS_DEBUG_
  Serial.println("Received Data :");
  #endif
  while(millis()<_timer+timeOut){
    if(Serial1.available()>0){
      char c = Serial1.available();

      msg[_cnt]=(uint8_t)Serial1.read();
      #ifdef _MODBUS_DEBUG_
      Serial.print(msg[_cnt],DEC);
      Serial.print(" ");
      #endif
      _cnt++;
      if(_cnt==len)break;
    }
  }
  #ifdef _MODBUS_DEBUG_
  Serial.println(" ");
  #endif
  //Serial1.flush();

  crc = calculateCRC(slaveID,(msg+1),len-3);
  if(crc==((msg[len-2]<<8)|msg[len-1])){
    //CRC IS CORRECT
    uint8_t byteCount = msg[2];
    if(byteCount==(registerCount*2)){
      for(int i = 0 ; i < registerCount;i++){
        output[i]=(msg[i*2+3]<<8)|msg[i*2+4];
      }
      return output;
    }
  }
  else{
    return 0;
  }
}

uint8_t ModbusMaster::clearBus(void){
  while(Serial1.available()){
    #ifdef _MODBUS_DEBUG_
      char c = Serial1.read();
      Serial.print(c);
      Serial.println(" ");
    #endif
    #ifndef _MODBUS_DEBUG_
      Serial1.read();
    #endif
  }

}
uint16_t ModbusMaster::calculateCRC(uint8_t address, uint8_t* pduFrame, uint8_t pduLen) {

	byte CRCHi = 0xFF, CRCLo = 0x0FF, Index;
  Index = CRCHi ^ address;
  CRCHi = CRCLo ^ _auchCRCHi[Index];
  CRCLo = _auchCRCLo[Index];

  while (pduLen--) {
      Index = CRCHi ^ *pduFrame++;
      CRCHi = CRCLo ^ _auchCRCHi[Index];
      CRCLo = _auchCRCLo[Index];
  }

  return (CRCHi << 8) | CRCLo;

}
