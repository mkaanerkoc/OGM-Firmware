#include "MKEModbusMaster.h"
#include "MKEBluetoothInterface.h"
#include "MKEBME280.h"
#include "MKEM24M02.h"
#include "MKESamplerLoader.h"
#include "MKETaskScheduler.h"
#include "Constants.h"
#include "UtilFunctions.h"
#include "LogFunctions.h"
#include "MotorFunctions.h"
#include "DeviceParams.h"

#include <Adafruit_SleepyDog.h>


#define DEVICE_CODE "61-2"
//#define _MKE_DEBUGGING_ 77
BluetoothInterface ble;
TaskScheduler ts;
ModbusMaster mm;
mEEPROM eeprom;
DeviceConfig devConfStr;
sampleLoader loader;
SamplerTimeConfig samplerConfStr;
TimeStruct tStr;
BME280 mySensor;
long sysTick=0;

char logBuffer[196];
char fname[10];
volatile static uint8_t PROGRAM_STATE = DATA_LOG_STATE;

//DEVICE PARAMETERS//

void tCallback(uint8_t tid){
  
  digitalClockDisplay();
  Serial.print("from task ");
  Serial.print(tid);
  Serial.println("  hello");
  Serial.println("*****************");
  ts.printTasks();
  Serial.println("*****************");
}


void bCurrentDatasCallback(byte *params){
  Serial.println("Datalar sorgulandi..");
  ReadAllSensors();
  Serial5.write("X");
  
  for(int i = 0 ; i < strlen(logBuffer);i++){
    Serial5.write(logBuffer[i]);
    delay(1);
  }
  
  Serial5.write("Y");
}

void bFilesCallback(byte *params){
  
  uint8_t msgCode = params[1];
  Serial.println("File Callback Main");
  Serial.println((char*)params);
  if(msgCode==0x20){
    char fn[16];
    sprintf(fn,"%c%c%c%c%c%c%c%c%c%c%c",params[2],params[3],params[4],params[5],params[6],params[7],params[8],params[9],params[10],params[11],params[12]);
    Serial.println(fn);
    PROGRAM_STATE = FILE_TRANSFER_STATE;
    sendFileViaBluetooth("data",fn);
    PROGRAM_STATE = DATA_LOG_STATE;
    //sendFileViaBluetooth("data","test.txt");
  }
  else if(msgCode==0x21){
    
  }
  else if(msgCode==0x22){
    
  }
  else if(msgCode==0x23){
    
  }
  else if(msgCode==0x24){ // LIST FILE NAMES
    
  }
  
}
void bMotorTestCallback(byte *params){
  Serial.println("Motor Test Callback");
  handleMotorCommand(params);
  
}
void bDevStatCallback(byte *params){
  
  
}
void bDevConfCallback(byte* params){
  Serial.println("Device Configuration Callback");
  handleDeviceParamsCommand(params);
  
}
void bCalibsCallback(byte* params){
 
  
}
void bSamplerCallback(byte* params){
  Serial.println("sampling command");
  uint8_t newState = params[1];
  if(newState = 0x01){
    Serial.println("start sampling");
    loader.startSampleLoading(sampleLoadingReasonPeriodic);
  }
  else if(newState = 0x02){
    Serial.println("stop sampling");
    digitalWrite(CONTAINER_1_SENSOR_PIN,LOW);
  }
  
}
void bProgramStateCallback(byte *params){
  uint8_t newState = params[1];
  
  
}

void setup() {
  // put your setup code here, to run once:
  //setSyncProvider(getTeensy3Time);
  Serial.begin(115200);
  //while(!Serial);
  
  mm.config(9600);
  ble.config(115200);
  
  setBME280Sensor();
  initSDCard();
  eeprom.enable(36,39);
  loader.begin();
  loader.registerCallback(&cb);
  
  ble.setCallback(DEVICE_CONF_CALLBACK,&bDevConfCallback);
  ble.setCallback(CALIBRATION_CALLBACK,&bCalibsCallback);
  ble.setCallback(FILE_CALLBACK,&bFilesCallback);
  ble.setCallback(CURRENT_DATA_CALLBACK,&bCurrentDatasCallback);
  ble.setCallback(SAMPLER_CALLBACK,&bSamplerCallback);
  ble.setCallback(MOTOR_TEST_CALLBACK,&bMotorTestCallback);
  
  digitalClockDisplay();
  //Serial1.flush();
  logAtBoot();
  mm.clearBus();
  sysTick=millis();
}

void loop() {
  // put your main code here, to run repeatedly:
 
  switch(PROGRAM_STATE){
    case DATA_LOG_STATE:
      ble.receive();
      if(millis()>sysTick+SYSTICK_TIMER*60){
        sysTick=millis();  
        ReadAllSensors();
        sprintf(fname,"%02d-%02d-%02d.da",day(),month(),(year()%2000));
        writeToFile("data",fname,logBuffer);
      }
      break;
    case SAMPLING_STATE:
      ble.receive();
      loader.operationHandler();
    default:
      break;
  }
  //TEST//
 /* if(millis()>sysTick+SYSTICK_TIMER*20){
        sysTick=millis();  
        ReadAllSensors();
        sprintf(fname,"%02d-%02d-%02d.da",day(),month(),(year()%2000));
        writeToFile("data",fname,logBuffer);
  }*/

  
 

  
}

void ReadAllSensors(){
  //
  mm.clearBus();
  uint16_t *turb,*phAndTemp,*distance,*cond,*oxy;
  float turbidity,pH,waterTemp,conductivity,oxygen,tempC,humdC;
  uint16_t dist=0;
  bool isOxygenSensorReady= false;

  isOxygenSensorReady=mm.writeSingleRegister(OXYGEN_SENSOR_DEV_ID,OXYGEN_TRIG_REGISTER,OXYGEN_TRIG_VALUE,SENSOR_TIME_OUT);
  delay(DELAY_BTW_SENSOR);
  phAndTemp = mm.readHoldingRegisters(PH_SENSOR_DEV_ID,PH_SENSOR_PH_REGISTER,1,SENSOR_TIME_OUT);
  pH = (float)phAndTemp[0]/100;
  
  #ifdef _MKE_DEBUGGING_
  Serial.print("pH value is : ");
  Serial.println(pH);
  #endif
  delay(DELAY_BTW_SENSOR);

  turb = mm.readHoldingRegisters(TURBIDITY_SENSOR_DEV_ID,TURBIDTIY_REGISTER,2,SENSOR_TIME_OUT);
  turbidity = floatFromTwoRegister(true,turb[0],turb[1]);

  #ifdef _MKE_DEBUGGING_
  Serial.print("Turbidity : ");
  Serial.println(turbidity);
  #endif
  delay(DELAY_BTW_SENSOR);
  
  distance = mm.readHoldingRegisters(ULTRASONIC_SENSOR_DEV_ID,DISTANCE_REGISTER,1,SENSOR_TIME_OUT);
  dist = *distance;
  #ifdef _MKE_DEBUGGING_
  Serial.print("Distance : ");
  Serial.println(*distance);
  #endif
  delay(DELAY_BTW_SENSOR);
  
  cond    = mm.readInputRegisters(CONDUCTIVITY_SENSOR_DEV_ID,CONDUCTIVITY_REGISTER,4,SENSOR_TIME_OUT);
  conductivity = floatFromTwoRegister(true,cond[0],cond[1]);
  waterTemp = floatFromTwoRegister(true,cond[2],cond[3]);
  
  #ifdef _MKE_DEBUGGING_
  Serial.print("Conductivity : ");
  Serial.println(conductivity);
  Serial.print("Water Temperature : ");
  Serial.println(waterTemp);
  #endif
  delay(DELAY_BTW_SENSOR);

  if(isOxygenSensorReady){
    oxy    = mm.readHoldingRegisters(OXYGEN_SENSOR_DEV_ID,OXYGEN_PPM_REGISTER,1,SENSOR_TIME_OUT);
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
  
  sprintf(logBuffer, "%s;%02d:%02d:%02d;%.2f;%.2f;%.2f;%.2f;%d;%.2f;%.2f;%.2f;%.2f;%d",DEVICE_CODE,hour(),minute(),second(),tempC,humdC,pH,oxygen,dist,calculateFlowFromHeight(620,dist),turbidity,conductivity,waterTemp,0);
  #ifdef _MKE_DEBUGGING_
  Serial.println(logBuffer);
  #endif
  return;
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
  mySensor.begin();
}

void cb (operationState s) {
  switch (s) {
    case noOperation:
      Serial.println("Sample Loader ==> No Operation");
      Serial5.print("SL 10");
      samplerLog("No Operation");
      break;
    case noozleCleaning:
      Serial.println("Sample Loader ==> Noozle Cleaning");
      PROGRAM_STATE = SAMPLING_STATE;
      Serial5.print("SL 11");
      samplerLog("Noozle Cleaning");
      break;
    case pipeCleaningVacuum:
      Serial.println("Sample Loader ==> C Filling");
      Serial5.print("SL 12");
      samplerLog("C Filling");
      break;
    case pipeCleaningExtrude:
      Serial.println("Sample Loader ==> C Extruding");
      Serial5.print("SL 13");
      samplerLog("C Extruding");
      break;
    case sampleLoadingVacuum:
      Serial.println("Sample Loader ==> SL Vacuum");
      Serial5.print("SL 14");
      samplerLog("SL Vacuum");
      break;
    case sampleLoadingFilling:
      Serial.println("Sample Loader ==> SL Filling");
      Serial5.print("SL 15");
      samplerLog("SL Filling");
      break;
    case sampleLoadingExtrude:
      Serial.println("Sample Loader ==> SL Extruding");
      Serial5.print("SL 16");
      samplerLog("SL Extruding");
      break;
    case finalNoozleClean:
      Serial.println("Sample Loader ==> Final Noozle Cleaning");
      Serial5.print("SL 17");
      samplerLog("Final Noozle Cleaning");
      break;
    case acidLoading:
      Serial.println("Sample Loader ==> Acid Loading");
      Serial5.print("SL 18");
      samplerLog("Acid Loading");
      break;
    case acidPipeClean:
      Serial.println("Sample Loader ==> Acid Pipe Cleaning");
      Serial5.print("SL 19");
      samplerLog("Acid Pipe Cleaning");
      break;
    case finished:
      Serial.println("Sample Loader ==> Finished");
      Serial5.print("SL 20");
      samplerLog("Finished");
      PROGRAM_STATE = DATA_LOG_STATE;
      break;
    case canceled:
      Serial.println("Sample Loader ==> Cancelled");
      Serial5.print("SL 21");
      samplerLog("Cancelled");
      break;
  }
}

void getDeviceConfigurationStruct(){
  
}
void getSamplingTimesStruct(){
  
}
void getSamplingPercentagesStruct(){
  
}



