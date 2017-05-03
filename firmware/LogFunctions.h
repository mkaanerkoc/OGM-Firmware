#include <SD.h>
#include <SPI.h>

File myFile;
const int chipSelect = BUILTIN_SDCARD;

uint8_t initSDCard(){
  return SD.begin(chipSelect);
}
uint8_t WriteDataLog(uint8_t _day,uint8_t _month,uint8_t _year,char *buf){
  uint8_t b = 0;
  char fileName[10];
  sprintf(fileName,"02d-%02d-%02d",_day,_month,_year%2000);
  myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    uint8_t b = myFile.println(buf);
    myFile.close();
    Serial.println("done.");
  }
  return b;
}

uint8_t WriteSamplerLog(){
  return 1;
}

uint8_t WriteDeviceLog(){
  return 1;
}

