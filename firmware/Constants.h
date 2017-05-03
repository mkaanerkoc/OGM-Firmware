#ifndef CONSTANTS_h
#define CONSTANTS_h

#include <TimeLib.h>


#define SYSTICK_TIMER 1000

#define PH_SENSOR_DEV_ID 5
#define OXYGEN_SENSOR_DEV_ID 5
#define CONDUCTIVITY_SENSOR_DEV_ID 5
#define ULTRASONIC_SENSOR_DEV_ID 36
#define TURBIDITY_SENSOR_DEV_ID 40

#define PH_SENSOR_SERIAL_CODE 0x7E48
#define OXYGEN_SENSOR_SERIAL_CODE 
#define CONDUCTIVITY_SENSOR_SERIAL_CODE
#define ULTRASONIC_SENSOR_SERIAL_CODE
#define TURBIDITY_SENSOR_SERIAL_CODE 

#define PH_SENSOR_PH_REGISTER 2
#define PH_SENSOR_TEMP_REGISTER 3
#define OXYGEN_REGISTER 1
#define TURBIDTIY_REGISTER 4
#define DISTANCE_REGISTER 3

struct TimeStruct{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  uint8_t day;
  uint8_t month;
  uint8_t year;
};

struct DataLog{
  uint8_t site_id;
  struct TimeStruct time;
  float ph;
  float water_temperature;
  float oxygen;
  float turbidity;
  float conductivity;
  float distance; 

};


time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
void getDateTimeStructed(struct TimeStruct *str){
  
  str->hour=hour();
  str->minute=minute();
  str->second=second();
  str->day=day();
  str->month=month();
  str->year=year();
  return;
}
void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

float floatFromTwoRegister(bool reversed,uint16_t reg1,uint16_t reg2){
  float retVal = 0;
  uint8_t a = 0xff&(reg1>>8);
  uint8_t b = reg1&0xff;
  uint8_t c = 0xff&(reg2>>8);
  uint8_t d = reg2&0xff;
  uint8_t *bb = (uint8_t*)malloc(4);
  if(!reversed){
    bb[0]=b;
    bb[1]=a;
    bb[2]=d;
    bb[3]=c;
  }
  else{
    bb[0]=d;
    bb[1]=c;
    bb[2]=b;
    bb[3]=a;
  }
  memcpy(&retVal,bb, sizeof(retVal));
  free(bb);
  return retVal;
}





#endif


