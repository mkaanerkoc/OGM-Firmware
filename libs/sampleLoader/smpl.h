#ifndef smpl_h
#define smpl_h

#include <Arduino.h>

enum containerStatus : short unsigned int{
    container1FourSampleLeft = 1,
    container1ThreeSampleLeft = 2,
    container1TwoSampleLeft = 3,
    container1OneSampleLeft = 4,
    container1NoneSampleLeft = 5,
    container2FourSampleLeft = (1<<4),
    container2ThreeSampleLeft = (2<<4),
    container2TwoSampleLeft = (3<<4),
    container2OneSampleLeft = (4<<4),
    container2NoneSampleLeft = (5<<5)

};

#define PUMP_1_PIN 3
#define PUMP_2_PIN 8
#define PUMP_3_PIN 9
#define PUMP_4_PIN 25

#define MAIN_LINE_SENSOR_PIN 26
#define CONTAINER_0_SENSOR_PIN 27
#define CONTAINER_1_SENSOR_PIN 28
#define UNUSED_SENSOR_PIN 29

#define DIRECTIONAL_PIN_A0 4
#define DIRECTIONAL_PIN_B0 5
#define DIRECTIONAL_PIN_C0 6
#define DIRECTIONAL_PIN_D0 7

#define DIRECTIONAL_PIN_A1 10
#define DIRECTIONAL_PIN_B1 11
#define DIRECTIONAL_PIN_C1 12
#define DIRECTIONAL_PIN_D1 24

#define SAMPLE_LOADING_TIME 160L         /* In seconds */
#define ACID_LOADING_TIME_EVENT 45L
#define ACID_LOADING_TIME_PERIODIC 45L
#define ACID_REVERSE_TIME 40L
#define CANCEL_REVERSE_TIME 50L

#define SENSOR_FAILSAFE_TIMEOUT 100L
#define SENSOR_FAILSAFE_DEADZONE 3L

#define RELAY_PIN 2

#define FROM_CASE false
#define TO_CASE true

enum engineName : short unsigned int {
    pump1 = 0,
    pump2 = 1,
    pump3 = 2,
    pump4 = 3
};


enum pumpDirection : short unsigned int{
    pumpDirectionToTheContainer,
    pumpDirectionAwayFromContainer
};

enum operationState : short unsigned int{
    noOperation = 0,
    noozleCleaning = 1,
    pipeCleaningVacuum = 2,
    pipeCleaningExtrude = 3,
    sampleLoadingVacuum = 4,
    sampleLoadingFilling = 5,
    sampleLoadingExtrude = 6,
    finalNoozleClean = 7,
    acidLoading = 8,
    acidPipeClean = 9,
    finished = 10,
    canceled = 11
};

enum sampleLoadingReason : short unsigned int{
    sampleLoadingReasonPeriodic,
    sampleLoadingReasonEvent,
    sampleLoadingReasonNone
};

struct pump {
    unsigned int pin;
    unsigned int sensorPin;
    bool dir;
};


typedef void(*callback)(operationState);

class sampleLoader {
private:

    bool isOperationOngoing;

    pump selectedAcidPump;
    pump selectedWaterPump;

    sampleLoadingReason reason;

    pumpDirection pDir;
    pumpDirection aDir;

    unsigned int tunedFillingTime; //Two bytes

    unsigned long int time;
    unsigned long int travelTime;

    unsigned long int acidloadingTime;
    unsigned long int waterLoadingTime;

    pump selectWaterPump(sampleLoadingReason reason);
    pump selectAcidPump(sampleLoadingReason reason);
/*
    void startPumpSlowly(pump p, byte speed = 2);
    void stopPumpSlowly(pump p, byte speed = 2);
*/
    void setPumpDirection(pumpDirection dir);
    pumpDirection getPumpDirection(void);

    void setAcidPumpDirection(pumpDirection dir);
    pumpDirection getAcidPumpDirection(void);
/*
    void applyWaterPumpDirection(void);
    void applyAcidPumpDirection(void);
*/
    unsigned long int selectAcidLoadingTime(sampleLoadingReason reason);
    unsigned long int selectWaterLoadingTime(sampleLoadingReason reason);
/*
    void changePumpDirectionWhileRunning(pump p);
    void changeAcidPumpDirectionWhileRunning(pump p);
*/
    void setRelayisOn(bool state);

    bool isCallbackAssigned;
    //bool isSensorFailureCallbackAssigned;
    //bool isSensorFailure;

    unsigned int getTheTunedFillingTimeFromFlash();
    void setTheTunedFillingTimeToFlash();

    callback functionC;

public:

    sampleLoader(void);
    ~sampleLoader(void);

    operationState state;
    void begin(void);

    void startSampleLoading(sampleLoadingReason reason);

    /*
    containerStatus getContainerStatus(sampleLoadingReason reason);
    void setServiceContainersEmptied(void);
    */
    void operationHandler(void);

    void cancelSampleLoading(void);
    void emergencyStopSampleLoading(void);

    bool isLoading(void);

    void registerCallback(callback cb);

    //int requestForRecordedWaterLoadingTime();
    //int requestForRecordedAcidLoadingTime();

    void setAcidLoadingTime(unsigned long int t);
    void setWaterLoadingTime(unsigned long int t);

    //KAAN UPDATES :d //
    void setAllOutputs(bool on);
    void runMotor(pump &motor,bool dir);
    void stopMotor(pump &motor);
    void changeMotor(pump &motor);
    //void debugRun();
};

#endif
