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
				LPC_GPIO2->FIOSET |= (1 << 6); // to change LED, change GPIO port and bit translation
			}
			else {
				printf("Button not pushed\n");
				LPC_GPIO2->FIOCLR |= (1 << 6); // change this one too
			}
		}
}

#elif part == 2
int main() {
	while(true) {
		int joystick_pressed = (LPC_GPIO1->FIOPIN & 0x100000); //20th pin (Centre)
		
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
	}
}

#else
float convert(float adc_value) {
		// conversion is 1/1241, since 1V input reads 1241 analog
		return roundf((adc_value / 1241) * 100) / 100;
}

int main() {
	// PCONP by setting bit 12
	LPC_SC->PCONP |= (1 << 12); 

	// PINSEL, from slides
	LPC_PINCON->PINSEL1 &= ~(0x3 << 18); // clear bits 18, 19
	LPC_PINCON->PINSEL1 |=  (0x1 << 18); // set bit 18 = AD0.2

	// ADCR
	LPC_ADC->ADCR = (1 <<  2) |  // turn on analog input bit 2
	                (4 <<  8) |  // bit 8..15 sets clock divisor: 4 + 1 = 5,  25MHz / 5 = 5MHz     
	                (1 << 21);   // enable ADCR circuitry
	
	// reading
	while(true)
	{
		LPC_ADC->ADCR |= (1 << 24); // start conversion
    while((LPC_ADC->ADGDR & 0x80000000) == 0); // checking if bit 31 of ADGDR is set
    uint16_t ADC_Value = ((LPC_ADC->ADGDR & 0xFFF0) >> 4); // extract bits 15...4 of LPC_ADC->ADGDR;
    printf("value: %f\n", convert(ADC_Value));
	}
}

#endif
