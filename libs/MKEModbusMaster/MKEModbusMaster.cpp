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

void ModbusMaster::config(HardwareSerial* port,uint16_t baudRate) {
  this->_port=port;
  (*_port).begin(baudRate,SERIAL_8N1);

}

uint8_t ModbusMaster::Function6(uint8_t slaveID,uint16_t registerAddress,uint16_t registerValue){
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
  for(int i = 0 ; i < 8 ; i++){
    (*_port).write(msg[i]);
    msg[i]=0;
  }
  //(*_port).flush();
  len=8;
  uint16_t rcv_crc=0x0000;
  while(millis()<_timer+3000){
    if((*_port).available()>0){
      char c = (*_port).available();
      if((uint8_t)c!=len)
        return 0;
      for(int j=0;j<c;j++){
        msg[j]=(uint8_t)(*_port).read();
        delay(1);
      }
    }
    break;
  }
  crc = calculateCRC(slaveID,(msg+1),5);
  rcv_crc=(msg[len-2]<<8)|msg[len-1];
  if(crc==rcv_crc){
    return 1;
  }
  else{
    0;
  }
}
uint16_t* ModbusMaster::Function4(uint8_t slaveID,uint16_t registerAddress,uint16_t registerCount){

  free(msg);
  free(output);
  output = (uint16_t*)malloc(sizeof(uint16_t)*registerCount);
  for(int i = 0 ; i < registerCount;i++){
    output[i]=0xffff;
  }
  msg = (uint8_t*)malloc(sizeof(uint8_t)*8);
  if(!msg){
    //(*_port).println("An error occured..");
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
  //uint8_t ind=0;
  _timer=millis();
  for(int i = 0 ; i < 8 ; i++){
    (*_port).write(msg[i]);
    msg[i]=0;
  }

  free(msg);
  msg=(uint8_t*)malloc(sizeof(uint8_t)*len);
  while(millis()<_timer+3000){
    if((*_port).available()>0){
      char c = (*_port).available();
      if((uint8_t)c!=len)
        return output;
      for(int j=0;j<c;j++){
        msg[j]=(uint8_t)(*_port).read();
      }
      break;
    }
  }
  (*_port).flush();
  //PDU RECEIVED IF ANY
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
}

uint16_t* ModbusMaster::Function3(uint8_t slaveID,uint16_t registerAddress,uint16_t registerCount){

  free(msg);
  free(output);
  output = (uint16_t*)malloc(sizeof(uint16_t)*registerCount);
  for(int i = 0 ; i < registerCount;i++){
    output[i]=0xffff;
  }
  msg = (uint8_t*)malloc(sizeof(uint8_t)*8);
  if(!msg){
    Serial.println("An error occured..");
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
  //uint8_t ind=0;
  _timer=millis();
  Serial.print("Sent message : ");
  for(int i = 0 ; i < 8 ; i++){
    (*_port).write(msg[i]);
    Serial.print(msg[i],HEX);
    Serial.print(" ");
    msg[i]=0;
  }
  Serial.println("");

  free(msg);
  msg=(uint8_t*)malloc(sizeof(uint8_t)*len);
  while(millis()<_timer+3000){
    if((*_port).available()>0){
      //char c = (*_port).available();
      //if((uint8_t)c!=len)
        //return output;
      Serial.print("Received message : ");

      for(int j=0;j<len;j++){
        msg[j]=(uint8_t)(*_port).read();
        Serial.print(msg[j],HEX);
        Serial.print(" ");
      }
      Serial.println("");
      break;
    }
  }
  //(*_port).flush();
  //PDU RECEIVED IF ANY
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
