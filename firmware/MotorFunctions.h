

#define PUMP_1_ID 0x01
#define PUMP_2_ID 0x02
#define PUMP_3_ID 0x03
#define PUMP_4_ID 0x04
#define RELAY_ID  0x05

#define TURN_ON  0x01
#define TURN_OFF 0x00

#define TURN_INSIDE 0x00
#define TURN_OUTSIDE 0x01

#define PUMP_1_PIN 3
#define PUMP_2_PIN 8
#define PUMP_3_PIN 9
#define PUMP_4_PIN 25


#define DIRECTIONAL_PIN_A0 4
#define DIRECTIONAL_PIN_B0 5
#define DIRECTIONAL_PIN_C0 6
#define DIRECTIONAL_PIN_D0 7

#define DIRECTIONAL_PIN_A1 10
#define DIRECTIONAL_PIN_B1 11
#define DIRECTIONAL_PIN_C1 12
#define DIRECTIONAL_PIN_D1 24


void handleMotorCommand(byte *params){
  uint8_t motorID = params[1];
  uint8_t directionCommand = params[2];
  uint8_t turnOnOff = params[3];
  switch motorID{
    case RELAY_ID:
      if(directionCommand==TURN_INSIDE){
        if(turnOnOff == TURN_ON){
           digitalWrite(RELAY_PIN, HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(RELAY_PIN, LOW);
        }
      }
      else if(directionCommand==TURN_OUTSITE){
        if(turnOnOff == TURN_ON){
          digitalWrite(RELAY_PIN, HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(RELAY_PIN, LOW);
        }
      }
      break;
    case PUMP_1_ID:
      if(directionCommand==TURN_INSIDE){
          if(turnOnOff == TURN_ON){
            digitalWrite(DIRECTIONAL_PIN_A0,LOW);
            delay(10);
            digitalWrite(DIRECTIONAL_PIN_B0,HIGH);
            delay(50);
            digitalWrite(PUMP_1_PIN,HIGH);
          }
          else if(turnOnOff == TURN_OFF){
            digitalWrite(DIRECTIONAL_PIN_A0,LOW);
            delay(10);
            digitalWrite(DIRECTIONAL_PIN_B0,LOW);
            delay(50);
            digitalWrite(PUMP_1_PIN,LOW);
          }
        }
        else if(directionCommand==TURN_OUTSITE){
          if(turnOnOff == TURN_ON){
            digitalWrite(DIRECTIONAL_PIN_A0,HIGH);
            delay(10);
            digitalWrite(DIRECTIONAL_PIN_B0,LOW);
            delay(50);
            digitalWrite(PUMP_1_PIN,HIGH);
          }
          else if(turnOnOff == TURN_OFF){
            digitalWrite(DIRECTIONAL_PIN_A0,LOW);
            delay(10);
            digitalWrite(DIRECTIONAL_PIN_B0,LOW);
            delay(50);
            digitalWrite(PUMP_1_PIN,LOW);
          }
        }
      break;
    case PUMP_2_ID:
      if(directionCommand==TURN_INSIDE){
        if(turnOnOff == TURN_ON){
          digitalWrite(DIRECTIONAL_PIN_C0,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_D0,HIGH);
          delay(50);
          digitalWrite(PUMP_2_PIN,HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(DIRECTIONAL_PIN_C0,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_D0,LOW);
          delay(50);
          digitalWrite(PUMP_2_PIN,LOW);
        }
      }
      else if(directionCommand==TURN_OUTSITE){
        if(turnOnOff == TURN_ON){
          digitalWrite(DIRECTIONAL_PIN_C0,HIGH);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_D0,LOW);
          delay(50);
          digitalWrite(PUMP_2_PIN,HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(DIRECTIONAL_PIN_C0,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_D0,LOW);
          delay(50);
          digitalWrite(PUMP_2_PIN,LOW);
        }
      }
      break;
    case PUMP_3_ID:
      if(directionCommand==TURN_INSIDE){
        if(turnOnOff == TURN_ON){
          digitalWrite(DIRECTIONAL_PIN_A1,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_B1,HIGH);
          delay(50);
          digitalWrite(PUMP_3_PIN,HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(DIRECTIONAL_PIN_A1,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_B1,LOW);
          delay(50);
          digitalWrite(PUMP_3_PIN,LOW);
        }
      }
      else if(directionCommand==TURN_OUTSITE){
        if(turnOnOff == TURN_ON){
          digitalWrite(DIRECTIONAL_PIN_A1,HIGH);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_B1,LOW);
          delay(50);
          digitalWrite(PUMP_3_PIN,HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(DIRECTIONAL_PIN_A1,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_B1,LOW);
          delay(50);
          digitalWrite(PUMP_3_PIN,LOW);
        }
      }
      break;
    case PUMP_4_ID:
      if(directionCommand==TURN_INSIDE){
        if(turnOnOff == TURN_ON){
            digitalWrite(DIRECTIONAL_PIN_C1,LOW);
            delay(10);
            digitalWrite(DIRECTIONAL_PIN_D1,HIGH);
            delay(50);
            digitalWrite(PUMP_4_PIN,HIGH);
        }
        else if(turnOnOff == TURN_OFF){
            digitalWrite(DIRECTIONAL_PIN_C1,LOW);
            delay(10);
            digitalWrite(DIRECTIONAL_PIN_D1,LOW);
            delay(50);
            digitalWrite(PUMP_4_PIN,LOW);
        }
      }
      else if(directionCommand==TURN_OUTSITE){
        if(turnOnOff == TURN_ON){
          digitalWrite(DIRECTIONAL_PIN_C1,HIGH);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_D1,LOW);
          delay(50);
          digitalWrite(PUMP_4_PIN,HIGH);
        }
        else if(turnOnOff == TURN_OFF){
          digitalWrite(DIRECTIONAL_PIN_C1,LOW);
          delay(10);
          digitalWrite(DIRECTIONAL_PIN_D1,LOW);
          delay(50);
          digitalWrite(PUMP_4_PIN,LOW);
        }
      }
      break;
    default:
      break;
  }
}

