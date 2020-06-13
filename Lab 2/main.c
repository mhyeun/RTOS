#include <LPC17xx.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define part 4

#if part == 1
int main() {
		// set bits to output
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
				printf("Button pushed\n");
				LPC_GPIO1->FIOSET |= (1 << 28); // to change LED, change GPIO port and bit translation (in case we get asked by TA for lab demo)
			}
			else {
				printf("Button not pushed\n");
				LPC_GPIO1->FIOCLR |= (1 << 28);
			}
		}
}

#elif part == 2
int main() {
	while(true) {
		int joystick_pressed = (LPC_GPIO1->FIOPIN & 0x100000); //20th pin (Cntr)
		
		int joystick_north = (LPC_GPIO1->FIOPIN & 0x800000); // 23rd pin (1 with 23 following 0s) (A)
		int joystick_east = (LPC_GPIO1->FIOPIN & 0x1000000); // 24th pin (B)
		int joystick_south = (LPC_GPIO1->FIOPIN & 0x2000000); // 25th pin (C)
		int joystick_west = (LPC_GPIO1->FIOPIN & 0x4000000); // 26th pin (D)

		if (joystick_pressed == 0) {
			printf("pressed\n");
		}
		else {
			printf("not pressed\n");
		}
		
		if (joystick_north == 0) {
			printf("North and ");
		}
		else if (joystick_east == 0) {
			printf("East and ");
		}
		else if (joystick_south == 0) {
			printf("South and ");
		}
		else if (joystick_west == 0) {
			printf("West and ");
		}
		else {
			printf("Centre and ");
		}	
	}
}

// X_XX ____

#elif part == 3
int main() {
		// set bits to output
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
	}
}

#else
int main() {
	
}
#endif
