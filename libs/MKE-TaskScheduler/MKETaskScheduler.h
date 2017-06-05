#ifndef MKETaskScheduler_h
#define MKETaskScheduler_h


#include "Arduino.h"
#include "stdlib.h"

#define SINGLE 0x01
#define PERIODIC 0x02

#define STEP_TIME 1000
typedef void(*taskCallback)(uint8_t tid);

typedef struct Task{
  uint8_t taskID;
  taskCallback cb;
  uint8_t type;
  int32_t period;
  char *name;
  struct Task* next;
}Task;



class TaskScheduler {
  public:
    TaskScheduler();
    void addTask(uint8_t taskID,uint8_t type,char *name, int32_t trigTime,taskCallback tcb);
    void removeTask(uint8_t taskID);
    void printTasks();
    void runTasks(void);
    void start(void);
    void stop(void);
    void step(void);
 private:
   Task *head;
   Task *tail;
   long _timer_;


};


#endif
