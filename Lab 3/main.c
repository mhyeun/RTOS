#include <LPC17xx.h>
#include <stdio.h>
#include <cmsis_os2.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

void read_joystick(void *arg) {
	while(true) {
		LPC_GPIO1->FIOCLR |= (1 << 28);
		LPC_GPIO1->FIOCLR |= (1 << 29);
		LPC_GPIO1->FIOCLR |= (1 << 31);
		LPC_GPIO2->FIOCLR |= (1 << 2);
		
		int joystick_pressed = (LPC_GPIO1->FIOPIN & 0x100000); //20th pin (Centre)
		int joystick_north = (LPC_GPIO1->FIOPIN & 0x800000); // 23rd pin (1 with 23 following 0s) (A)
		int joystick_east = (LPC_GPIO1->FIOPIN & 0x1000000); // 24th pin (B)
		int joystick_south = (LPC_GPIO1->FIOPIN & 0x2000000); // 25th pin (C)
		int joystick_west = (LPC_GPIO1->FIOPIN & 0x4000000); // 26th pin (D)
		
		if (joystick_pressed == 0) {
			LPC_GPIO2->FIOSET |= (1 << 3); // LED 5
		}
		else {
			LPC_GPIO2->FIOCLR |= (1 << 3); //turn off LED 5
		}
		
		if (joystick_north == 0) {
			LPC_GPIO1->FIOSET |= (1 << 28); // LED 1 0x1
		}
		else if (joystick_east == 0) {
			LPC_GPIO1->FIOSET |= (1 << 29); // LED 2 0x10
		}
		else if (joystick_south == 0) {
			LPC_GPIO1->FIOSET |= (1 << 28); // LED 1 0x11
			LPC_GPIO1->FIOSET |= (1 << 29); // LED 2
		}
		else if (joystick_west == 0) {
			LPC_GPIO1->FIOSET |= (1 << 31); // LED 3 0x100
		}
		else {
		}
		osDelay(50);
	}
}

void read_ADC(void *arg) {
	//initialize
	LPC_GPIO1->FIODIR |= (1 << 28);
	LPC_GPIO1->FIODIR |= (1 << 29);
	LPC_GPIO1->FIODIR |= (1 << 31);
	LPC_GPIO2->FIODIR |= (1 << 2);
	LPC_GPIO2->FIODIR |= (1 << 3);
	LPC_GPIO2->FIODIR |= (1 << 4);
	LPC_GPIO2->FIODIR |= (1 << 5);
	LPC_GPIO2->FIODIR |= (1 << 6);
	
	while(true){		
		char str[100];
		printf( "Enter a value from 0 to 255: ");
		scanf("%s", str);
		
		// turn off LEDs before starting computation
		LPC_GPIO1->FIOCLR |= (1 << 28);
		LPC_GPIO1->FIOCLR |= (1 << 29);
		LPC_GPIO1->FIOCLR |= (1 << 31);
		LPC_GPIO2->FIOCLR |= (1 << 2);
		LPC_GPIO2->FIOCLR |= (1 << 3);
		LPC_GPIO2->FIOCLR |= (1 << 4);
		LPC_GPIO2->FIOCLR |= (1 << 5);
		LPC_GPIO2->FIOCLR |= (1 << 6);
		
		// for ASCII conversion, subtract 48 from digits
		// e.g. "9" -> 57 in ASCII, so subtract 48 to get 9
		int num = atoi(str);
		if (num > 255 || num < 0){
			printf("Invalid input. Please try again.\n");
			continue;
		}
		
		for(int i = 0; num > 0; i++){
			if(num % 2 == 1) {
				if(i == 0) LPC_GPIO1->FIOSET |= (1 << 28);
				else if(i == 1) LPC_GPIO1->FIOSET |= (1 << 29);
				else if(i == 2) LPC_GPIO1->FIOSET |= (1 << 31);
				else if(i == 3) LPC_GPIO2->FIOSET |= (1 << 2);
				else if(i == 4) LPC_GPIO2->FIOSET |= (1 << 3);
				else if(i == 5) LPC_GPIO2->FIOSET |= (1 << 4);
				else if(i == 6) LPC_GPIO2->FIOSET |= (1 << 5);
				else LPC_GPIO2->FIOSET |= (1 << 6);
			}
			num = floor(num / 2);
		}
		osThreadYield();
	}
}

void toggle_LED(void *arg) {
	//initialize
	LPC_GPIO1->FIODIR |= (1 << 28);
	LPC_GPIO1->FIODIR |= (1 << 29);
	LPC_GPIO1->FIODIR |= (1 << 31);
	LPC_GPIO2->FIODIR |= (1 << 2);
	LPC_GPIO2->FIODIR |= (1 << 3);
	LPC_GPIO2->FIODIR |= (1 << 4);
	LPC_GPIO2->FIODIR |= (1 << 5);
	LPC_GPIO2->FIODIR |= (1 << 6);

	// turn off LEDs
	LPC_GPIO1->FIOCLR |= (1 << 28);
	LPC_GPIO1->FIOCLR |= (1 << 29);
	LPC_GPIO1->FIOCLR |= (1 << 31);
	LPC_GPIO2->FIOCLR |= (1 << 2);
	LPC_GPIO2->FIOCLR |= (1 << 3);
	LPC_GPIO2->FIOCLR |= (1 << 4);
	LPC_GPIO2->FIOCLR |= (1 << 5);
	LPC_GPIO2->FIOCLR |= (1 << 6);

	while(true) {
		int button_pushed = (LPC_GPIO2->FIOPIN & 0x400); // 10th pin
		
		if (button_pushed == 0) {
			LPC_GPIO2->FIOSET |= (1 << 6); // to change LED, change GPIO port and bit translation
		}
		else {
			LPC_GPIO2->FIOCLR |= (1 << 6); // change this one too
		}
		osDelay(50);
	}
}

int main() {
	osKernelInitialize();
	osThreadNew(read_joystick, NULL, NULL);
	osThreadNew(read_ADC, NULL, NULL);
	osThreadNew(toggle_LED, NULL, NULL);
	osKernelStart();
}
