/*	Scheduler
	Created by Eugenjus Margalikas, May 22, 2019
*/

#ifndef Sched_int_h
#define Sched_int_h

#include "Arduino.h"

#define TASK_COUNT 8
struct SchTask {
	void (*task)(int);
	int param;
	unsigned long lastActive;
	unsigned long interval;
	int repeatCount;
	bool repeat;
	bool empty;
	SchTask();
	SchTask(void (*task)(int),int param,unsigned long lastActive, unsigned long interval, int repeatCount, bool repeat, bool empty);
	SchTask(void(*task)(int),int param);
	//SchTask(void(*task)(),unsigned long interval);
	//SchTask(void(*task)(),unsigned long interval, unsigned long repeatCount);
	SchTask* withInterval(unsigned long interval);
	SchTask* withRepeatCount(int repeatCount);
	SchTask* repeatOnce();
	SchTask* withShift(unsigned long shift);
};

class Scheduler{
	public:
		Scheduler();
		SchTask* addTask(void (*task)(int),int param);
		SchTask* _addTask(void (*task)(int));
		SchTask taskList[TASK_COUNT];
		void run();
		int emptyCount();
		
};


#endif