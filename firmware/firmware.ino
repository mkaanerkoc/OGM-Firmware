#include "MKEModbusMaster.h"
#include "MKEBluetoothInterface.h"
#include "MKEBME280.h"
#include "Constants.h"
#include "UtilFunctions.h"


BluetoothInterface ble;
ModbusMaster mm;
TimeStruct tStr;
BME280 mySensor;
long sysTick=0;

void setup() {
  // put your setup code here, to run once:
  mm.config(&Serial1,9600);
  ble.config(115200);
  setSyncProvider(getTeensy3Time);
  setBME280Sensor();

  sysTick=millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  ble.receive();
  if(millis()>sysTick+SYSTICK_TIMER*10){
    sysTick=millis();
    Serial.println("");
    digitalClockDisplay();
    ReadAllSensors();


    
  }
}

String ReadAllSensors(){
  //
  char logBuffer[196];
  uint16_t *turb,*phAndTemp,*distance,*cond,*oxy;
  float turbidity,pH,waterTemp,conductivity,oxygen,tempC,humdC;
  bool isOxygenSensorReady= false;
    
  isOxygenSensorReady=mm.Function6(OXYGEN_SENSOR_DEV_ID,OXYGEN_TRIG_REGISTER,OXYGEN_TRIG_VALUE);
  delay(5);
  phAndTemp = mm.Function3(PH_SENSOR_DEV_ID,PH_SENSOR_PH_REGISTER,2);
  pH = (float)phAndTemp[0]/100;
  waterTemp = (float)phAndTemp[1]/10;
  #ifdef _MKE_DEBUGGING_
  Serial.print("pH value is : ");
  Serial.println(pH);
  Serial.print("Water Temp is : ");
  Serial.println(waterTemp);
  #endif
  delay(5);

  turb = mm.Function3(TURBIDITY_SENSOR_DEV_ID,TURBIDTIY_REGISTER,2);
  turbidity = floatFromTwoRegister(true,turb[0],turb[1]);

  #ifdef _MKE_DEBUGGING_
  Serial.print("Turbidity : ");
  Serial.println(turbidity);
  #endif
  delay(5);

  distance = mm.Function3(ULTRASONIC_SENSOR_DEV_ID,DISTANCE_REGISTER,1);
  #ifdef _MKE_DEBUGGING_
  Serial.print("Distance : ");
  Serial.println(*distance);
  #endif
  delay(5);
  
  cond    = mm.Function4(CONDUCTIVITY_SENSOR_DEV_ID,CONDUCTIVITY_REGISTER,2);
  conductivity = floatFromTwoRegister(true,cond[0],cond[1]);
  
  #ifdef _MKE_DEBUGGING_
  Serial.print("Conductivity : ");
  Serial.println(conductivity);
  #endif
  delay(5);

  if(isOxygenSensorReady){
    oxy    = mm.Function3(OXYGEN_SENSOR_DEV_ID,OXYGEN_PPM_REGISTER,1);
    oxygen = (float)(*oxy)/100;
    #ifdef _MKE_DEBUGGING_
    Serial.print("Oxygen in PPM : ");
    Serial.println(oxygen);
    #endif
  }
  tempC = mySensor.readTempC();
  humdC = mySensor.readFloatHumidity();
  #ifdef _MKE_DEBUGGING_
  Serial.print("Temperature: ");
  Serial.print(tempC, 2);
  Serial.println(" degrees C");

  Serial.print("%RH: ");
  Serial.print(humdC, 2);
  Serial.println(" %");
  Serial.println("");
  #endif
  sprintf(logBuffer, "%02d:%02d:%02d;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f;%.2f",hour(),minute(),second(),pH,conductivity,turbidity,oxygen,calculateFlowFromHeight(670,*distance),waterTemp,tempC,humdC);
  Serial.println(logBuffer);
  return "";
}

void setBME280Sensor(){
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x77;
  mySensor.settings.runMode = 3; //Normal mode
  mySensor.settings.tStandby = 0;
  mySensor.settings.filter = 0;
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;
  Serial.println(mySensor.begin(), HEX);
}

void showBME280Values(){
  
  
  Serial.println();
  
}

