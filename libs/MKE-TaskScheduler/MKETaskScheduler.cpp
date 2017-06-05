#include "MKETaskScheduler.h"

TaskScheduler::TaskScheduler() {
    this->head=NULL;
    this->tail=NULL;
}


void TaskScheduler::addTask(uint8_t taskID,uint8_t type,char *name, int32_t trigTime,taskCallback tcb) {
    Task *newTask = NULL;
    newTask = (Task*)malloc(sizeof(Task));
    newTask->taskID = taskID;
    newTask->type = type;
    newTask->name = name;
    newTask->period = trigTime;
    newTask->cb = tcb;
    newTask->next = NULL;
    if(this->head==NULL){
      this->head=newTask;
      this->tail = newTask;
    }
    else{
      this->tail->next=newTask;
      this->tail=newTask;
    }
}

void TaskScheduler::removeTask(uint8_t taskid){
  Task *task=head;
  Task *cursor;
  if(task==NULL){
    return;
  }
  /* check for the head */
  if(task->taskID==taskid){
    head = task->next;
    free(task);
    return;
  }

  /*                   */
  while(task->next!=NULL){
    cursor = task->next;
    if(cursor->taskID == taskid){
      if(cursor->next!=NULL)
        task->next = cursor->next;
      else
        task->next=NULL;
      free(cursor);
      return;
    }
    task = task->next;
  }
}

void TaskScheduler::printTasks(void){
    Task *task = head;
    if(task==NULL){
      Serial.println("This list is not initialized");
      return;
    }
    Serial.print("Head Task ID is : ");
    Serial.println(task->taskID);
    Serial.print("Head Task name is : ");
    Serial.println(task->name);
    Serial.print("Remained task time : ");
    Serial.println(task->period);
    while(task->next!=NULL){
      task = task->next;
      Serial.print("Task ID is : ");
      Serial.println(task->taskID);
      Serial.print("Task name is : ");
      Serial.println(task->name);
      Serial.print("Remained task time : ");
      Serial.println(task->period);
    }
    return;
}

void TaskScheduler::runTasks(void){
    Task *task = head;
    if(task==NULL){
      Serial.println("This list is not initialized");
    }
    while(task!=NULL){
      task->cb(task->taskID);
      task = task->next;
    }
}
void TaskScheduler::start(void){
  this->_timer_=millis();
}
void TaskScheduler::stop(void){

}

void TaskScheduler::step(void){
  if(millis()>(_timer_+ STEP_TIME )){
    _timer_=millis();
    Task *task = head;
    Task *cursor;
    if(task==NULL){
      Serial.println("all tasks done");
      return;
    }
    // CHECK FOR HEAD //
    if(STEP_TIME > task->period ){
       task->cb(task->taskID);
       this->head = task->next;
       free(task);
    }
    else{
      task->period = task->period - STEP_TIME;
    }

    while(task->next!=NULL){
      cursor = task->next;
      if(STEP_TIME > cursor->period ){
         cursor->cb(cursor->taskID);
         if(cursor->type==SINGLE){
           if(cursor->next!=NULL){
             task->next = cursor->next;
           }
           else{
             task->next=NULL;
             return;
           }
           free(cursor);
         }

      }
      else{
        cursor->period = cursor->period - STEP_TIME;
      }
      task = task->next;
    }
  }
}
