#include <cmsis_os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "random.h"
#include "lfsr113.h"

typedef struct {
  uint8_t Buf[32];
  uint8_t Idx;
} MSGQUEUE_OBJ_t;


osMessageQueueId_t q_1;
osMessageQueueId_t q_2;

int seconds = 0, previous_seconds = 0;
int queue1Sent = 0, queue2Sent = 0;
int queue1Overflows = 0, queue2Overflows = 0;
int queue1Received = 0, queue2Received = 0;

void server(void *arg) {
	bool is_queue_1 = true;

	// From API
	MSGQUEUE_OBJ_t msg;
	msg.Buf[0] = 0x55U;
  msg.Idx  = 0U;
	
	while(true){
		// Calculate delay
		int delay = osKernelGetTickFreq() * next_event();
		delay /= 10;
		delay = delay >> 16;
		osDelay(delay);
		
		if(q_1 == (osMessageQueueId_t *) arg) {
			is_queue_1 = true;
		} else {
			is_queue_1 = false;
		}
		
		if(osMessageQueueGet((osMessageQueueId_t *) arg, &msg, NULL, osWaitForever) == osOK) {
			if(is_queue_1) {
				queue1Received++;
			} else {
			queue2Received++;
			}
		}
	}
}

void client(void *arg) {
	bool receiver_is_q1 = true;
	
	// From API
	MSGQUEUE_OBJ_t msg;
	msg.Buf[0] = 0x55U;
  msg.Idx  = 0U;
	
	while(true){
		// Calculate delay
		int delay = osKernelGetTickFreq() * next_event();
		delay /= 9 * 2;
		delay = delay >> 16;
		osDelay(delay);
		
		int queue_count = receiver_is_q1 ? osMessageQueueGetCount(q_1) : osMessageQueueGetCount(q_2);

		if(receiver_is_q1) {
			if(osMessageQueuePut(q_1, &msg, 0, 0) == osErrorResource) {
				queue1Overflows++;
			} else {
				queue1Sent++;
			}
		} else {
			if(osMessageQueuePut(q_2, &msg, 0, 0) == osErrorResource) {
				queue2Overflows++;
			} else {
				queue2Sent++;
			}
		}
		receiver_is_q1 = receiver_is_q1 ? false : true;
		osThreadYield();
	}
}	

// below two functions are "monitor" together
void update_time(void *arg) {
	while(true){
		osDelay(1000);
		seconds++;
	}
}

void print(void *arg) {
	while(true){
		if (seconds > previous_seconds) {
			previous_seconds = seconds;
			
			// print results
			printf("Time: %d, Sent q_1: %d, Sent q_2: %d, Receieved q_1: %d, Received q_2: %d, Overflowed q_1: %d, Overflowed q_2: %d\n",
			seconds, queue1Sent, queue2Sent, queue1Received, queue2Received, queue1Overflows, queue2Overflows);
		}
	}
}

int main() {
	osKernelInitialize();
	q_1 = osMessageQueueNew(10, sizeof(int), NULL);
	q_2 = osMessageQueueNew(10, sizeof(int), NULL);
	
	osThreadNew(client, NULL, NULL);
	
	osThreadNew(server, q_1, NULL);
	osThreadNew(server, q_2, NULL);
	
	osThreadNew(update_time, NULL, NULL);
	osThreadNew(print, NULL, NULL);
	
	osKernelStart();
	for(;;);
}
