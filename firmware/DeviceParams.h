/*#define DEVICE_ID_ADDR  10
#define DEVICE_ID_LENGTH 1
#define FIELD_ID_ADDR 11 
#define FIELD_ID_LENGTH 1

#define PH_LOW_ADDR 20
#define PH_LOW_LENGTH 4
#define PH_HIGH_ADDR 24
#define PH_HIGH_LENGTH 4

#define OXYGEN_LOW_ADDR 30
#define OXYGEN_LOW_LENGTH 4
#define OXYGEN_HIGH_ADDR 34
#define OXYGEN_HIGH_LENGTH 4

#define TURBIDITY_LOW_ADDR
#define TURBIDITY_LOW_LENGTH 4
#define TURBIDITY_HIGH_ADDR
#define TURBIDITY_HIGH_LENGTH 4

#define CONDUCTIVITY_LOW_ADDR
#define CONDUCTIVITY_LOW_LENGTH 4
#define CONDUCTIVITY_HIGH_ADDR
#define CONDUCTIVITY_HIGH_LENGTH 4

#define FLOW_LOW_ADDR
#define FLOW_LOW_LENGTH 4
#define FLOW_HIGH_ADDR
#define FLOW_HIGH_LENGTH 4

#define WATER_TEMP_LOW_ADDR
#define WATER_TEMP_LOW_LENGTH 4
#define WATER_TEMP_HIGH_ADDR
#define WATER_TEMP_HIGH_LENGTH 4

#define RAIN_HIGH_ADDR
#define RAIN_HIGH_LENGTH 4
#define RAIN_WAIT_TIME_ADDR
#define RAIN_WAIT_TIME_LENGTH 1


#define TIME_SAMPLING_WATER_TIME_ADDR
#define TIME_SAMPLING_WATER_TIME_LENGTH 2
#define EVENT_SAMPLING_WATER_TIME_ADDR
#define EVENT_SAMPLING_WATER_TIME_LENGTH 2 

#define TIME_SAMPLING_ACIDE_TIME_ADDR
#define TIME_SAMPLING_ACIDE_TIME_LENGTH 2
#define EVENT_SAMPLING_ACIDE_TIME_ADDR 
#define EVENT_SAMPLING_ACIDE_TIME_LENGTH 2

#define READ_PERIOD_ADDR 
#define READ_PERIOD_LENGTH 1

#define NOTCH_HEIGHT_ADDR 
#define NOTCH_HEIGHT_LENGTH 1

#define PERIODIC1_SAMPLE_TIME_ADDR 
#define PERIODIC1_SAMPLE_TIME_LENGTH 3

#define PERIODIC2_SAMPLE_TIME_ADDR 
#define PERIODIC2_SAMPLE_TIME_LENGTH 3*/

#define DEVICE_ID_REQ 0
#define SITE_ID_REQ 4
#define PH_LOWER_REQ 20
#define PH_HIGHER_REQ 24
#define CONDUCTIVITY_LOWER_REQ 28
#define CONDUCTIVITY_HIGHER_REQ 32
#define TURBIDITY_LOWER_REQ 36
#define TURBIDITY_HIGHER_REQ 40
#define OXYGEN_LOWER_REQ 44
#define OXYGEN_HIGHER_REQ 48
#define RAIN_HIGHER_REQ 52
#define TIMEOUT_EVENT_REQ 56
#define REFERENCE_HEIGHT_REQ 60
#define READ_PERIOD_REQ 64

#define TIME_BASED_TOTAL_REQ 80
#define TIME_BASED_PERC_REQ 84
#define EVENT_BASED_TOTAL_REQ 88
#define EVENT_BASED_PERC_REQ 92

#define CONFIG_PARAMS 0x00
#define SAMPLING_TIME_PARAMS 0x10
#define DATETIME_PARAMS  0x20
#define SAMPLING_PERC_PARAMS 0x30

#define WRITE_FUNCTION 0x02
#define READ_FUNCTION 0x01

#define ALL_PARAMS 0x00
#define SINGLE_PARAMS 0x01


void handleDeviceParamsCommand(byte *params,mEEPROM eeprom,uint16_t msgLen,DeviceConfig *dvc,SamplerTimeConfig *stc,SamplerPercConfig *spc){

  uint8_t messageLength = msgLen;
  uint8_t typeCode = params[1];
  uint8_t msgCode,funcCode,paramCode;
  char configBuffer[24];  
  char devParamBuffer[160];
  char singleParamBuffer[8];
  float fParam;
  uint16_t u16Param;
  uint8_t u8Param;
  //SamplerTimeConfig (*stc);
  //DeviceConfig (*dvc);
  
  switch (typeCode){
    case CONFIG_PARAMS:
      msgCode = params[2];
      funcCode = params[3];
      if(msgCode == ALL_PARAMS){
        if(funcCode == READ_FUNCTION){
           eeprom.readConfigStruct(dvc);
           sprintf(devParamBuffer,"%d;%d;%2.2f;%2.2f;%2.2f;%2.2f;%2.2f;%2.2f;%2.2f;%2.2f;%2.2f;%d;%d;%d;%d;%d;%d;%d",(*dvc).deviceID,(*dvc).fieldID,(*dvc).phLowThr,(*dvc).phHighThr,(*dvc).conductivityLowThr,(*dvc).conductivityHighThr,
              (*dvc).turbidityLowThr,(*dvc).turbidityHighThr,(*dvc).oxygenLowThr,(*dvc).oxygenHighThr,(*dvc).rainHighThr,(*dvc).eventTimeOut,(*dvc).timeBasedWaterSamplingTime,(*dvc).eventBasedWaterSamplingTime,(*dvc).timeBasedAcideSamplingTime,
              (*dvc).eventBasedAcideSamplingTime,(*dvc).readPeriod,(*dvc).notchHeight);
            Serial5.print("X");
            delay(100);
            for(int i = 0 ; i < strlen(devParamBuffer);i++){
              Serial5.write(devParamBuffer[i]);
            }
            delay(100);
            Serial5.print("Y");
            return;
        }
        else if(funcCode == WRITE_FUNCTION){
          
        }
      }
      else if(msgCode==SINGLE_PARAMS){
        
        paramCode = params[3];
        funcCode = params[4];
        eeprom.readConfigStruct(dvc);
        sprintf(singleParamBuffer,"%.*s", (messageLength-5), params + 5);
        Serial.println(singleParamBuffer);
        if(paramCode==DEVICE_ID_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            // GET THE PARAMETER STRING MANUALLY //
            u8Param = atoi(singleParamBuffer);
            (*dvc).deviceID = u8Param;
          }
        }
        else if(paramCode==SITE_ID_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            u8Param = atoi(singleParamBuffer);
            (*dvc).fieldID = u8Param;
          }
        }
        else if(paramCode==PH_LOWER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).phLowThr = fParam;
          }
        }
        else if(paramCode==PH_HIGHER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).phHighThr = fParam;
          }
        }
        else if(paramCode==CONDUCTIVITY_LOWER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            
            fParam = atof(singleParamBuffer);
            (*dvc).conductivityLowThr = fParam;
          }
        }
        else if(paramCode==CONDUCTIVITY_HIGHER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).conductivityHighThr = fParam;
          }
        }
        else if(paramCode==TURBIDITY_LOWER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).turbidityLowThr = fParam;
          }
        }
        else if(paramCode==TURBIDITY_HIGHER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).turbidityHighThr = fParam;
          }
        }
        else if(paramCode==OXYGEN_LOWER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).oxygenLowThr = fParam;
          }
        }
        else if(paramCode==OXYGEN_HIGHER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
             fParam = atof(singleParamBuffer);
             (*dvc).oxygenHighThr = fParam;
          }
        }
        else if(paramCode==RAIN_HIGHER_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            fParam = atof(singleParamBuffer);
            (*dvc).rainHighThr = fParam;
          }
        }
        else if(paramCode==TIMEOUT_EVENT_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            u16Param = atoi(singleParamBuffer);
            (*dvc).eventTimeOut = u16Param;
          }
        }
        else if(paramCode==REFERENCE_HEIGHT_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            u16Param = atoi(singleParamBuffer);
            (*dvc).notchHeight = u16Param;
          }
        }
        else if(paramCode==READ_PERIOD_REQ)
        {
          if(funcCode==READ_FUNCTION){
            
          }
          else if(funcCode==WRITE_FUNCTION){
            u8Param = atoi(singleParamBuffer);
            (*dvc).readPeriod = u8Param;
          }
        }
      }
      eeprom.writeConfigStruct((*dvc));
      Serial5.print("OK");
      break;
    case SAMPLING_TIME_PARAMS:
      funcCode = params[2];
      
      if(funcCode==WRITE_FUNCTION){
        Serial.println("Write Sampling Times");
        (*stc).weekDay1 = params[3];
        (*stc).hour1 = params[4];
        (*stc).minute1 = params[5];
        (*stc).weekDay2 = params[6];
        (*stc).hour2 = params[7];
        (*stc).minute2 = params[8];
        eeprom.writeSamplerTimeStruct((*stc));
        Serial5.print("OK");
     }
     else if(funcCode==READ_FUNCTION){
       Serial.println("Read Sampling Times");
       eeprom.readSamplerTimeStruct(stc);
       sprintf(configBuffer,"%d;%d;%d;%d;%d;%d",(*stc).weekDay1,(*stc).hour1,(*stc).minute1,(*stc).weekDay2,(*stc).hour2,(*stc).minute2);
       Serial5.print("X");
       delay(100);
       for(int i = 0 ; i < strlen(configBuffer);i++){
          Serial5.write(configBuffer[i]);
       }
       delay(100);
       Serial5.print("Y");
     }
      break;
    case SAMPLING_PERC_PARAMS:
      funcCode = params[2];
      msgCode = params[3];
      eeprom.readSamplerPercStruct(spc);
      if(funcCode == 0x01){
        if(msgCode==READ_FUNCTION){
          sprintf(configBuffer,"X%d;%d;%dY",(*spc).periodicTotal,(*spc).periodicWater,(*spc).periodicAcide);
          for(int i = 0 ; i < strlen(configBuffer);i++){
              Serial5.write(configBuffer[i]);
          }
        }
        else if(msgCode == WRITE_FUNCTION){
          uint16_t pTotal = (params[4]<<8)|(params[5]);
          uint16_t pWater = (params[6]<<8)|(params[7]);
          (*spc).periodicTotal = pTotal;
          (*spc).periodicWater = pWater;
          (*spc).periodicAcide = pTotal - pWater;
          eeprom.writeSamplerPercStruct((*spc));
          Serial5.print("OK");
        }
      }
      else if(funcCode == 0x02){
        if(msgCode==READ_FUNCTION){
          sprintf(configBuffer,"X%d;%d;%dY",(*spc).eventTotal,(*spc).eventWater,(*spc).eventAcide);
          for(int i = 0 ; i < strlen(configBuffer);i++){
              Serial5.write(configBuffer[i]);
          }
        }
        else if(msgCode == WRITE_FUNCTION){
          uint16_t eTotal = (params[4]<<8)|(params[5]);
          uint16_t eWater = (params[6]<<8)|(params[7]);
          (*spc).eventTotal = eTotal;
          (*spc).eventWater = eWater;
          (*spc).eventAcide = eTotal - eWater;
          eeprom.writeSamplerPercStruct((*spc));
          Serial5.print("OK");
        }
      }
      break;
    case DATETIME_PARAMS:
      funcCode = params[2];
      if(funcCode == WRITE_FUNCTION){
        setTime(params[3],params[4],params[5],params[6],params[7],params[8]);
        Serial5.print("OK");
      }
      else if(funcCode == READ_FUNCTION){
        sprintf(configBuffer,"X%02d:%02d:%02d %02d-%02d-%dY",hour(),minute(),second(),day(),month(),year());
        for(int i = 0 ; i < strlen(configBuffer);i++){
            Serial5.write(configBuffer[i]);
        }
      }
      break;
    default:
      break;
  }
}



