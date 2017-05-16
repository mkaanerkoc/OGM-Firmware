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

#define WRITE_FUNCTION 
#define READ_FUNCTION

#define ALL_PARAMS 0x00
#define SINGLE_PARAMS 0x01


void handleDeviceParamsCommand(byte *params){

  uint8_t typeCode = params[1];
  uint8_t msgCode,funcCode,paramCode;
  char configBuffer[24];  
  SamplerTimeConfig samplerConfStr;
  DeviceConfig devConfStr;
  
  switch typeCode{
    case CONFIG_PARAMS:
      msgCode = params[2];
      funcCode = params[3];
      if(msgCode == ALL_PARAMS){
        if(funcCode == READ_FUNCTION){
          
        }
        else(funcCode == WRITE_FUNCTION){
          
        }
      }
      else(msgCode==SINGLE_PARAMS){
        else if(paramCode==DEVICE_ID_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==FIELD_ID_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==PH_LOWER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==PH_HIGHER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==CONDUCTIVITY_LOWER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==CONDUCTIVITY_HIGHER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==TURBIDITY_LOWER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==TURBIDITY_HIGHER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==OXYGEN_LOWER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==OXYGEN_HIGHER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==RAIN_HIGHER_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==TIMEOUT_EVENT_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==REFERENCE_HEIGHT_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        else if(paramCode==READ_PERIOD_REQ)
        {
          if(fCode==READ_FUNCTION){
            
          }
          else if(fCode==WRITE_FUNCTION){
            
          }
        }
        
      }
      break;
    case SAMPLING_TIME_PARAMS:
      if(fCode==0x02){
        Serial.println("Write Sampling Times");
        samplerConfStr.weekDay1 = params[3];
        samplerConfStr.hour1 = params[4];
        samplerConfStr.minute1 = params[5];
        samplerConfStr.weekDay2 = params[6];
        samplerConfStr.hour2 = params[7];
        samplerConfStr.minute2 = params[8];
        eeprom.writeSamplerTimeStruct(samplerConfStr);
        Serial5.print("OK");
     }
     else if(fCode==0x01){
       eeprom.readSamplerTimeStruct(&samplerConfStr);
       sprintf(configBuffer,"%d;%d;%d;%d;%d;%d",samplerConfStr.weekDay1,samplerConfStr.hour1,samplerConfStr.minute1,samplerConfStr.weekDay2,samplerConfStr.hour2,samplerConfStr.minute2);
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
      
      break;
    case DATETIME_PARAMS:
      funcCode = params[2];
      if(funcCode == READ_FUNCTION){
        setTime(params[3],params[4],params[5],params[6],params[7],params[8]);
        Serial5.print("OK");
      }
      else if(funcCode == WRITE_FUNCTION){
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



