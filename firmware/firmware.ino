#include "MKEModbusMaster.h"
#include "MKEBluetoothInterface.h"
#include "Constants.h"


BluetoothInterface ble;
ModbusMaster mm;
TimeStruct tStr;

long sysTick=0;

void setup() {
  // put your setup code here, to run once:
  mm.config(&Serial1,9600);
  //Serial1.begin(9600);
  ble.config(115200);
  setSyncProvider(getTeensy3Time);

  Serial.println("hello world");
  sysTick=millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  //ble.receive();
  if(millis()>sysTick+SYSTICK_TIMER*5){
    sysTick=millis();
    ReadAllSensors();


    
  }
}

String ReadAllSensors(){
  //
  Serial.println("");
  uint16_t *turb,*phAndTemp,*distance;
  float turbidity,pH,waterTemp;
    digitalClockDisplay();
  phAndTemp = mm.Function3(PH_SENSOR_DEV_ID,PH_SENSOR_PH_REGISTER,2);
  pH = (float)phAndTemp[0]/10;
  waterTemp = (float)phAndTemp[1]/100;
  Serial.print("pH value is : ");
  Serial.println(pH);
  Serial.print("Water Temp is : ");
  Serial.println(waterTemp);

  delay(5);

  turb = mm.Function3(TURBIDITY_SENSOR_DEV_ID,TURBIDTIY_REGISTER,2);
  turbidity = floatFromTwoRegister(true,turb[0],turb[1]);

  Serial.print("Turbidity : ");
  Serial.println(turbidity);

  delay(5);

 distance = mm.Function3(ULTRASONIC_SENSOR_DEV_ID,DISTANCE_REGISTER,1);
 Serial.print("Distance : ");
 Serial.println(*distance);
  
  
  return "";
}


