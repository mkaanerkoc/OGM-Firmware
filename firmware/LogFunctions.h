#include <SD.h>
#include <SPI.h>

File myFile;
const int chipSelect = BUILTIN_SDCARD;

uint8_t initSDCard(){
  return SD.begin(chipSelect);
}
uint8_t WriteDataLog(uint8_t _day,uint8_t _month,uint8_t _year,char *buf){
  uint8_t b = 0;
  char fileName[12];
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

void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}


void writeToFile(const char *dir,const char* filename, char *data){
  char fileName[32];
  if(!SD.exists(dir)){
    SD.mkdir(dir);
  }
  File file = SD.open(dir);
  sprintf(fileName,"%s/%s",dir,filename);
  file = SD.open(fileName, FILE_WRITE);
  file.println(data);
  file.close();
}
void readFromFile(const char *dir,const char* filename){
  char fileName[32];
  if(!SD.exists(dir)){
    SD.mkdir(dir);
  }
  File file = SD.open(dir);
  sprintf(fileName,"%s/%s",dir,filename);
  file = SD.open(fileName, FILE_READ);
  while (file.available()) {
      Serial.write(file.read());
  }
}

void sendFileViaBluetooth(const char *dir,const char* filename){
  delay(1000);
  volatile uint32_t i=0;
  char fileName[32];
  if(!SD.exists(dir)){
    return;
  }
  File file = SD.open(dir);
  sprintf(fileName,"%s/%s",dir,filename);
  if(SD.exists(fileName)){
    Serial5.print("X");
    delay(200);
    file = SD.open(fileName, FILE_READ);
    while (file.available()) {
        Serial5.write(file.read());
        delay(1);
        
    }
    delay(200);
    Serial5.print("Y");
  }
  else{
    Serial5.print("qwe");
  }
  
}

void sendFileNamesViaBluetooth(const char *dirr){
  File dir = SD.open(dirr);
  while(true){
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       break;
     }
     Serial5.write("Q");
     if (!entry.isDirectory()) {
        // files have sizes, directories do not
       Serial5.print(entry.name());
       Serial5.print(";");
       Serial5.println(entry.size(), DEC);
     } 
     entry.close();
     Serial5.write("W");
   }
}
void logAtBoot(){
  char fileName[32];
  char logRow[32];
  sprintf(fileName,"%02d-%02d-%02d.dd",day(),month(),(year()%2000));
  sprintf(logRow,"Boot at : %02d:%02d:%02d",hour(),minute(),second());
  writeToFile("device",fileName,logRow);
}

void samplerLog(const char *log_){
  char fileName[32];
  char logRow[48];
  sprintf(fileName,"%02d-%02d-%02d.sa",day(),month(),(year()%2000));
  sprintf(logRow,"%s at : %02d:%02d:%02d",log_,hour(),minute(),second());
  writeToFile("sampler",fileName,logRow);
}

void getLastDataFile(){
  File dir = SD.open("data");
  File lastFile;
  char lastFileName[24];
  while(true){
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       Serial.printf("Last File Name : %s\n",lastFile.name());
       break;
     }
     lastFile = entry;
     
  }
}

void getLastRowsFromFile(uint8_t rowCount){
  File dir = SD.open("data");
  File lastFile;
  char lastFileName[24];
  while(true){
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       Serial.printf("Last File Name : %s\n",lastFile.name());
       // BURAYI DOLDUR AGA//
       if(lastFile.size()<65*(rowCount+1)){
        while(lastFile.available()){
          char c = lastFile.read();
          Serial.print(c);
        }
       }
       else{
        lastFile.seek(lastFile.size()-65*(rowCount+1));
        lastFile.readStringUntil('\n');
        while(lastFile.available()){
          char c = lastFile.read();
          Serial.print(c);
        }
       }
       

       
       return;
     }
     lastFile = entry;
     
  }
}



