#include "Arduino.h"
#include "Scheduler_int_param.h"

SchTask* Scheduler::_addTask(void(*task)(int)){
	for (int i = 0; i<TASK_COUNT; i++){
		if (taskList[i].empty){
			taskList[i]=SchTask(task,i);
			return &taskList[i];
		}
	}
	return NULL;
}

SchTask* Scheduler::addTask(void(*task)(int),int param){
	for (int i = 0; i<TASK_COUNT; i++){
		if (taskList[i].empty){
			taskList[i]=SchTask(task,param);
			return &taskList[i];
		}
	}
	return NULL;
}

Scheduler::Scheduler(){
	for (int i = 0; i<TASK_COUNT; i++){
		taskList[i].empty=true;
	}
}
void Scheduler::run(){
	unsigned long currTime = millis();
	Serial.println(currTime);
	for (int i = 0; i<TASK_COUNT; i++){
		Serial.print(taskList[i].lastActive);
		Serial.print(" ");
		Serial.println( taskList[i].empty);
			if ( !(taskList[i].empty) && ((currTime - taskList[i].lastActive) >= taskList[i].interval) ){
				Serial.println("task run");
				taskList[i].task(taskList[i].param);
				if (!taskList[i].repeat || taskList[i].repeatCount==1) {
					taskList[i].empty=true;
					Serial.println("task done");
				}else{
					taskList[i].repeatCount= taskList[i].repeatCount- ( (taskList[i].repeatCount == 0) ? 0:1);
					taskList[i].lastActive=currTime;
				}
			}
	}
}

int Scheduler::emptyCount(){
	int res=0;
	for(int i =0; i<TASK_COUNT; i++){
		res=res + taskList[i].empty?1:0;
	}
	return res;
}	

SchTask::SchTask(){
	this->task=NULL;
	this->param=0;
	this->lastActive=0;
	this->interval=0;
	this->repeatCount=0;
	this->repeat=false;
	this->empty=true;		
}
SchTask::SchTask(void (*task)(int),int param,unsigned long lastActive, unsigned long interval, int repeatCount, bool repeat, bool empty){
	Serial.println("full constructor");
	this->task=task;
	this->param=param;
	this->lastActive=lastActive;
	this->interval=interval;
	this->repeatCount=repeatCount;
	this->repeat=repeat;
	this->empty=empty;
}
SchTask::SchTask(void(*task)(int),int param){
	
	this->task=task;
	this->param=param;
	this->lastActive=0;
	this->interval=0;
	this->repeatCount=0;
	this->repeat=false;
	this->empty=false;		
	Serial.println("task constructor");
}

SchTask* SchTask::withInterval(unsigned long interval){
	
	if (this!=NULL){Serial.println("withInterval");
		this->interval=interval;
		return this;
	}
	return NULL;
}
SchTask* SchTask::withRepeatCount(int repeatCount){
	
	if (this!=NULL){Serial.println("withRepeatCount");
		this->repeatCount=repeatCount;
		this->repeat=true;
		return this;
	}
	return NULL;
}

SchTask* SchTask::repeatOnce(){
	return this->withRepeatCount(1);	
}

SchTask* SchTask::withShift(unsigned long shift){
	this->lastActive=this->lastActive+shift;
	return this;
}
