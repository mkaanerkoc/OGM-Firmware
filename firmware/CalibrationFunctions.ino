#define PH_CALIBRATION 0x01
#define OXYGEN_CALIBRATION 0x02
#define CONDUCTIVITY_CALIBRATION 0x03
#define TURBIDITY_CALIBRATION 0x04
#define WATER_TEMPERATURE_CALIBRATION 0x05



#define START_CALIBRATION 0x01
#define STOP_CALIBRATION 0x02
#define DO_CALIBRATION 0x03
#define RESET_CALIBRATION 0x04



uint8_t activeSensor=0x00;


void handleCalibration(){
  
}
void handleCalibrationCommands(byte *params,uint16_t leng,ModbusMaster mm){
  uint8_t sensorCode = params[1];
  uint8_t funcCode   = params[2];
 
  if(sensorCode == PH_CALIBRATION){
    if(funcCode == START_CALIBRATION){
      
    }
    else if(funcCode == STOP_CALIBRATION){
      
    }
    else if(funcCode == DO_CALIBRATION){
      
    }
    else if(funcCode == RESET_CALIBRATION){
      
    }
  }
  else if(sensorCode == OXYGEN_CALIBRATION){
    if(funcCode == START_CALIBRATION){
      
    }
    else if(funcCode == STOP_CALIBRATION){
      
    }
    else if(funcCode == DO_CALIBRATION){
      
    }
    else if(funcCode == RESET_CALIBRATION){
      
    }
  }
  else if(sensorCode == CONDUCTIVITY_CALIBRATION){
    if(funcCode == START_CALIBRATION){
      
    }
    else if(funcCode == STOP_CALIBRATION){
      
    }
    else if(funcCode == DO_CALIBRATION){
      
    }
    else if(funcCode == RESET_CALIBRATION){
      
    }
  }
  else if(sensorCode == TURBIDITY_CALIBRATION){
    if(funcCode == START_CALIBRATION){
      
    }
    else if(funcCode == STOP_CALIBRATION){
      
    }
    else if(funcCode == DO_CALIBRATION){
      
    }
    else if(funcCode == RESET_CALIBRATION){
      
    }
  }
  else if(sensorCode == WATER_TEMPERATURE_CALIBRATION){
    if(funcCode == START_CALIBRATION){
      
    }
    else if(funcCode == STOP_CALIBRATION){
      
    }
    else if(funcCode == DO_CALIBRATION){
      
    }
    else if(funcCode == RESET_CALIBRATION){
      
    }
  }
  else{
    
  }
  
}
void ReadPHSensor(ModbusMaster mm){
  
  
}
void ReadOxyenSensor(ModbusMaster mm){
  
  
}
void ReadTurbiditysSensor(ModbusMaster mm){
  
  
}
void ReadConductivitySensor(ModbusMaster mm){
  
  
}

