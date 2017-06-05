#include "MKESamplerLoader.h"

sampleLoader::sampleLoader(){

}

sampleLoader::~sampleLoader(){

}


pump sampleLoader::selectWaterPump(sampleLoadingReason reason){
        pump p;

        if(reason == sampleLoadingReasonEvent) {
                p.pin = PUMP_1_PIN;
                p.sensorPin = CONTAINER_0_SENSOR_PIN;
        }else{
                p.pin = PUMP_3_PIN;
                p.sensorPin = CONTAINER_1_SENSOR_PIN;
        }
        return p;
}

pump sampleLoader::selectAcidPump(sampleLoadingReason reason){
        pump p = this->selectedWaterPump;
        pump c;
        if(p.pin == PUMP_1_PIN) {
                c.pin = PUMP_2_PIN;
        }else{
                c.pin = PUMP_4_PIN;
        }
        return c;
}

void sampleLoader::setPumpDirection(pumpDirection dir){
        this->pDir = dir;
}

pumpDirection sampleLoader::getPumpDirection(void){
        return this->pDir;
}

void sampleLoader::setAcidPumpDirection(pumpDirection dir){
        this->aDir = dir;
}

pumpDirection sampleLoader::getAcidPumpDirection(void){
        return this->aDir;
}


void sampleLoader::setRelayisOn(bool state){
        if(state) {
                digitalWrite(RELAY_PIN, HIGH);
        }else{
                digitalWrite(RELAY_PIN, LOW);
        }
}

void sampleLoader::begin(){

        this->state = noOperation;
        this->reason = sampleLoadingReasonNone;

        this->time = 0;
        this->travelTime = 0;

        this->pDir = pumpDirectionAwayFromContainer;
        this->aDir = pumpDirectionAwayFromContainer;

        this->isCallbackAssigned = false;

        this->acidloadingTime = 45L;
        this->waterLoadingTime = 160L;

        pinMode(RELAY_PIN, OUTPUT);
        digitalWrite(RELAY_PIN, HIGH);

        pinMode(PUMP_1_PIN, OUTPUT);
        pinMode(PUMP_2_PIN, OUTPUT);
        pinMode(PUMP_3_PIN, OUTPUT);
        pinMode(PUMP_4_PIN, OUTPUT);

        pinMode(MAIN_LINE_SENSOR_PIN, INPUT_PULLUP);
        pinMode(CONTAINER_0_SENSOR_PIN, INPUT_PULLUP);
        pinMode(CONTAINER_1_SENSOR_PIN, INPUT_PULLUP);
        pinMode(UNUSED_SENSOR_PIN, INPUT_PULLUP);

        pinMode(DIRECTIONAL_PIN_A0, OUTPUT);
        pinMode(DIRECTIONAL_PIN_B0, OUTPUT);
        pinMode(DIRECTIONAL_PIN_C0, OUTPUT);
        pinMode(DIRECTIONAL_PIN_D0, OUTPUT);
        pinMode(DIRECTIONAL_PIN_A1, OUTPUT);
        pinMode(DIRECTIONAL_PIN_B1, OUTPUT);
        pinMode(DIRECTIONAL_PIN_C1, OUTPUT);
        pinMode(DIRECTIONAL_PIN_D1, OUTPUT);

        digitalWrite(DIRECTIONAL_PIN_A0, LOW);
        digitalWrite(DIRECTIONAL_PIN_B0, LOW);
        digitalWrite(DIRECTIONAL_PIN_C0, LOW);
        digitalWrite(DIRECTIONAL_PIN_D0, LOW);

        digitalWrite(DIRECTIONAL_PIN_A1, LOW);
        digitalWrite(DIRECTIONAL_PIN_B1, LOW);
        digitalWrite(DIRECTIONAL_PIN_C1, LOW);
        digitalWrite(DIRECTIONAL_PIN_D1, LOW);


}

void sampleLoader::startSampleLoading(sampleLoadingReason reason){

        if(this->state == noOperation) {
                this->reason = reason;
                setRelayisOn(false);

                //select water and acid pumps

                this->selectedWaterPump = this->selectWaterPump(reason);
                delay(50);
                this->selectedAcidPump = this->selectAcidPump(reason);
                delay(50);


                runMotor(this->selectedWaterPump,FROM_CASE);

                this->time = millis();

                this->state = noozleCleaning;
                if(this->isCallbackAssigned) {
                        this->functionC(this->state);
                }
        }
}



void sampleLoader::operationHandler(){
        unsigned long int cTime = millis();
        switch (this->state) {
        case noozleCleaning:
                if (cTime >= (this->time + 15000)) {
                        this->time = millis();
                        this->state = pipeCleaningVacuum;
                        changeMotor(this->selectedWaterPump);
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }
                }
                break;
        case pipeCleaningVacuum:
                delay(1000);
                if (digitalRead(MAIN_LINE_SENSOR_PIN) == LOW) {
                        //if (cTime >= (this->time + 45000)){
                        if(cTime > (SENSOR_FAILSAFE_DEADZONE * 1000 + this->time)) {
                                //get the travel time
                                this->travelTime = cTime - this->time;
                                //change the direction of the pump
                                changeMotor(this->selectedWaterPump);
                                //update state
                                this->state = pipeCleaningExtrude;
                                //update time
                                this->time = millis();
                                if(this->isCallbackAssigned) {
                                        this->functionC(this->state);
                                }
                                break;
                        }else{
                                //sensor Failure
                        }
                }
                if (cTime > (SENSOR_FAILSAFE_TIMEOUT*1000L + this->time)) {
                        //sensor failure change state
                        //get the travel time
                        this->travelTime = SENSOR_FAILSAFE_TIMEOUT * 1000L;
                        //change the direction of the pump
                        changeMotor(this->selectedWaterPump);
                        //update state
                        this->state = pipeCleaningExtrude;
                        //update time
                        this->time = millis();

                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }
                }

                break;
        case pipeCleaningExtrude:
                if(cTime > (this->time + 5000 + this->travelTime)) {
                        changeMotor(this->selectedWaterPump);
                        //update time
                        this->time = millis();
                        //update state
                        this->state = sampleLoadingVacuum;
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }
                }
                break;
        case sampleLoadingVacuum:
                //Wait for water to reach the pump
                delay(1000);
                if(digitalRead(MAIN_LINE_SENSOR_PIN) == LOW) {
                        if(cTime > (SENSOR_FAILSAFE_DEADZONE * 1000 + this->time)) {
                                //Water has raised to the pump update state
                                this->state = sampleLoadingFilling;
                                //Get a referance to the time
                                this->time = millis();
                                if(this->isCallbackAssigned) {
                                        this->functionC(this->state);
                                }
                        }else{
                                //sensor Failure
                        }
                }
                if (cTime > ((SENSOR_FAILSAFE_TIMEOUT * 1000L) + (this->time))) {
                        //Water has raised to the pump update state
                        this->state = sampleLoadingFilling;
                        //Get a referance to the time
                        this->time = millis();
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }
                }

                break;
        case sampleLoadingFilling:
                //Look for the time
                if(cTime > ((this->waterLoadingTime * 1000L) + this->time)) {
                        changeMotor(this->selectedWaterPump);
                        //Record the time reference
                        time = millis();
                        //update the state
                        state = sampleLoadingExtrude;

                        if(isCallbackAssigned) {
                                this->functionC(state);
                        }

                }
                break;
        case sampleLoadingExtrude:
                if(cTime > (this->travelTime + this->time)) {
                        //pipe is clean continue to the next stage
                        this->state = finalNoozleClean;
                        //Record the time reference
                        this->time = millis();
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }
                }
                break;
        case finalNoozleClean:
                if(cTime > (this->time + 15000L)) {
                        //Water pump has reached its most deserved retirement in the sequence
                        //this->stopPumpSlowly(this->selectedWaterPump,5);
                        stopMotor(this->selectedWaterPump);
                        //Update state
                        this->state = acidLoading;
                        //select the direction for the acid
                        /* this->setAcidPumpDirection(pumpDirectionToTheContainer);
                           //apply the direction for the acid
                           this->applyAcidPumpDirection();*/
                        //start the acid pump
                        //this->startPumpSlowly(this->selectedAcidPump,5);
                        runMotor(this->selectedAcidPump,TO_CASE);
                        //update time reference
                        this->time = millis();
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }

                }
                break;
        case acidLoading:
                //check time
                if (cTime >(this->time + (1000L * this->acidloadingTime))) {
                        //reverse the acid pump
                        //this->changeAcidPumpDirectionWhileRunning(this->selectedAcidPump);
                        changeMotor(this->selectedAcidPump);
                        //update the state
                        this->state = acidPipeClean;
                        //update time referance
                        this->time = millis();
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }

                }
                break;
        case acidPipeClean:
                if (cTime > (this->time + (ACID_REVERSE_TIME * 1000L))) {
                        //shut down the acid pump
                        //this->stopPumpSlowly(this->selectedAcidPump);
                        stopMotor(this->selectedAcidPump);
                        //Update state to finished
                        this->state = finished;
                        if(this->isCallbackAssigned) {
                                this->functionC(this->state);
                        }

                }
                break;
        case finished:
                //TO-DO add a callback function !!!
                this->state = noOperation;
                //shut down the relay
                this->setRelayisOn(true);
                //wait for system to stabilize
                delay(200);
                //clear the time
                this->time = 0;

                // reset(false);

                if(this->isCallbackAssigned) {
                        this->functionC(this->state);
                }
                break;
        case canceled:
                if(cTime > (this->time + (long int)(CANCEL_REVERSE_TIME * 1000))) {
                        //shut down both motors
                        //this->stopPumpSlowly(this->selectedWaterPump,5);
                        //this->stopPumpSlowly(this->selectedAcidPump,5);
                        stopMotor(this->selectedAcidPump);
                        stopMotor(this->selectedWaterPump);
                        //update the finished state
                        this->state = finished;
                        this->travelTime = 0;
                        this->time = 0;
                        this->reason = sampleLoadingReasonNone;
                }
        default:
                break;
        }
        delay(1000);
        if (digitalRead(this->selectedWaterPump.sensorPin) == LOW){
          //Container Full cancel immedietly
            if(this->state != canceled){
              this->cancelSampleLoading();
            }
        }
}

void sampleLoader::cancelSampleLoading(){
        //get current state
        int currentS = static_cast<int>(this->state);
        //find which pump is working
        if (currentS <= 7) {
                //Currently water pump is working stop it.
                this->stopMotor(this->selectedWaterPump);
        }else{
                //same for acid pump
                this->stopMotor(this->selectedAcidPump);
        }
        runMotor(this->selectedAcidPump, FROM_CASE);
        runMotor(this->selectedWaterPump, FROM_CASE);
        //take a reference to the time
        this->time = millis();
        //take state to canceled
        this->state = canceled;
        //call the callback
        if(this->isCallbackAssigned) {
                this->functionC(this->state);
        }
}


void sampleLoader::emergencyStopSampleLoading(){
        // cut the board supply immedietly
        this->setRelayisOn(true);
        // clear the time
        this->time = 0;
        // set the state
        this->state = finished;
        //call the callback
        if(this->isCallbackAssigned) {
                this->functionC(this->state);
        }
}

bool sampleLoader::isLoading(){
        bool a;
        if(this->state != noOperation) {
                a = true;
        }else{
                a = false;
        }
        return a;
}

void sampleLoader::registerCallback(callback cb){
        this->functionC = cb;
        this->isCallbackAssigned = true;
}


void sampleLoader::setAcidLoadingTime(unsigned long int t){
        this->acidloadingTime = t;
}

void sampleLoader::setWaterLoadingTime(unsigned long int t){
        this->waterLoadingTime = t;
}


void sampleLoader::runMotor(pump &motor,bool dir){
        this->setAllOutputs(false);
        motor.dir=dir;
        delay(500);
        switch(motor.pin) {
        case PUMP_1_PIN:
                if(dir) {
                        digitalWrite(DIRECTIONAL_PIN_A0,LOW);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_B0,HIGH);
                        delay(50);
                        digitalWrite(PUMP_1_PIN,HIGH);
                }
                else{
                        digitalWrite(DIRECTIONAL_PIN_A0,HIGH);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_B0,LOW);
                        delay(50);
                        digitalWrite(PUMP_1_PIN,HIGH);
                }
                break;
        case PUMP_2_PIN:
                if(dir) {
                        digitalWrite(DIRECTIONAL_PIN_C0,LOW);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_D0,HIGH);
                        delay(50);
                        digitalWrite(PUMP_2_PIN,HIGH);
                }
                else{
                        digitalWrite(DIRECTIONAL_PIN_C0,HIGH);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_D0,LOW);
                        delay(50);
                        digitalWrite(PUMP_2_PIN,HIGH);
                }
                break;
        case PUMP_3_PIN:
                if(dir) {
                        digitalWrite(DIRECTIONAL_PIN_A1,LOW);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_B1,HIGH);
                        delay(50);
                        digitalWrite(PUMP_3_PIN,HIGH);
                }
                else{
                        digitalWrite(DIRECTIONAL_PIN_A1,HIGH);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_B1,LOW);
                        delay(50);
                        digitalWrite(PUMP_3_PIN,HIGH);
                }
                break;
        case PUMP_4_PIN:
                if(dir) {
                        digitalWrite(DIRECTIONAL_PIN_C1,LOW);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_D1,HIGH);
                        delay(50);
                        digitalWrite(PUMP_4_PIN,HIGH);
                }
                else{
                        digitalWrite(DIRECTIONAL_PIN_C1,HIGH);
                        delay(10);
                        digitalWrite(DIRECTIONAL_PIN_D1,LOW);
                        delay(50);
                        digitalWrite(PUMP_4_PIN,HIGH);
                }
                break;
        default:
                break;
        }
}
void sampleLoader::stopMotor(pump &motor){
        this->setAllOutputs(false);
        delay(500);
        switch(motor.pin) {
        case PUMP_1_PIN:
                digitalWrite(DIRECTIONAL_PIN_A0,LOW);
                delay(10);
                digitalWrite(DIRECTIONAL_PIN_B0,LOW);
                delay(50);
                digitalWrite(PUMP_1_PIN,LOW);
                break;
        case PUMP_2_PIN:
                digitalWrite(DIRECTIONAL_PIN_C0,LOW);
                delay(10);
                digitalWrite(DIRECTIONAL_PIN_D0,LOW);
                delay(50);
                digitalWrite(PUMP_2_PIN,LOW);
                break;
        case PUMP_3_PIN:
                digitalWrite(DIRECTIONAL_PIN_A1,LOW);
                delay(10);
                digitalWrite(DIRECTIONAL_PIN_B1,LOW);
                delay(50);
                digitalWrite(PUMP_3_PIN,LOW);
                break;
        case PUMP_4_PIN:
                digitalWrite(DIRECTIONAL_PIN_C1,LOW);
                delay(10);
                digitalWrite(DIRECTIONAL_PIN_D1,LOW);
                delay(50);
                digitalWrite(PUMP_4_PIN,LOW);
                break;
        default:
                break;
        }
}
void sampleLoader::changeMotor(pump &motor){
        motor.dir = !motor.dir;
        stopMotor(motor);
        delay(1000);
        runMotor(motor,motor.dir);
}

void sampleLoader::setAllOutputs(bool on){
        digitalWrite(DIRECTIONAL_PIN_A0,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_B0,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_C0,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_D0,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_A1,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_B1,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_C1,LOW);
        delay(50);
        digitalWrite(DIRECTIONAL_PIN_D1,LOW);
        delay(50);
        digitalWrite(PUMP_1_PIN,on);
        delay(50);
        digitalWrite(PUMP_2_PIN,on);
        delay(50);
        digitalWrite(PUMP_3_PIN,on);
        delay(50);
        digitalWrite(PUMP_4_PIN,on);
}
