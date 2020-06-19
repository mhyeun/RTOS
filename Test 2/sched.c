/**
 * Implement scheduleEDF().
 * @author Matthew Hyeun (20774652)
 */

#include "sched.h"

/**
 * Create task execution order.  Assigns task numbers to slots.  Uses
 * non-preemptive EDF scheduling.
 * @param slots array representing task execution order
 * @param nSlot number of entries in slots
 * @param tasks array of tasks (the taskset)
 * @param nTask size of the taskset
 * @param LCM length of the fixed schedule
 * @return true on success, false on failure
 */
 
// define c_assert
#define c_assert(e) ((e) ? (true) : \
		(printf("%s,%d: assertion '%s' failed\n", \
		__FILE__, __LINE__, #e), false))

// checks if LCM is correct
bool isLCMCorrect(int LCM, task_t tasks[], int nTask) {
	for(int i = 0; i < nTask; i++) {
		if(LCM % tasks[i].T != 0) {
			return false;
		}
	}
	return true;
}

// checks if nSlot is correct
bool isnSlotCorrect(int LCM, task_t tasks[], int nTask, int nSlot) {
	int calculatedNSlot = 0;
	for(int i = 0; i < nTask; i++) {
		calculatedNSlot += (LCM / tasks[i].T);
	}
	return nSlot == calculatedNSlot;
}

// calculates minimum value in an array, returns its index
// i.e. calculates the earliest deadline that is available
// i.e. not the one that just ran
int earliestDeadline(int nextDeadline[], int nTask, int indexOfLastTaskRan) {
	int temp = 0;
	
	if(indexOfLastTaskRan != -1) {
		temp = nextDeadline[indexOfLastTaskRan];
		nextDeadline[indexOfLastTaskRan] = 99999999; //assume no deadlines will exceed this value
	}
	
	int min = nextDeadline[0];
	int minIndex = 0;
	for(int task = 1; task < nTask; task++) {
		if (nextDeadline[task] < min) {
			min = nextDeadline[task];
			minIndex = task;
		}
	}
	
	if(indexOfLastTaskRan != -1) {
		nextDeadline[indexOfLastTaskRan] = temp;
	}
	return minIndex;
}

// main function to schedule EDF
bool scheduleEDF(int slots[], int nSlot, task_t tasks[], int nTask, int LCM) {
	if(!isLCMCorrect(LCM, tasks, nTask)) {
		c_assert(isLCMCorrect(LCM, tasks, nTask));
		return false;
	}
	if(!isnSlotCorrect(LCM, tasks, nTask, nSlot)) {
		c_assert(isnSlotCorrect(LCM, tasks, nTask, nSlot));
		return false;
	}
	
	// array to hold next upcoming deadline for each task in order
	int nextDeadline[nTask];
	for(int task = 0; task < nTask; task++) {
		nextDeadline[task] = tasks[task].T;
	}
	
	int time = 0;
	int slot = 0;
	int indexOfLastTaskRan = -1;
	while(time < LCM) {
			// determines which task to be run
			int indexOfTaskToRun = earliestDeadline(nextDeadline, nTask, indexOfLastTaskRan); 

			// adds task to slot, increment slot index
			slots[slot] = indexOfTaskToRun;
			slot++;
		
			// update next time the run task is going to be available
			nextDeadline[indexOfTaskToRun] += tasks[indexOfTaskToRun].T;
		
			// increment time by task time duration
			time += tasks[indexOfTaskToRun].C;
		
			// update indexOfLastTaskRan
			indexOfLastTaskRan = indexOfTaskToRun;
	}
	
	return true;
}
