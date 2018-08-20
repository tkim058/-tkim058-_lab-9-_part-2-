/*
 * [tkim058]_lab[9]_part[2].c
 * Partner(s) Name & E-mail: MinHwan Oh
 * Lab Section: B01
 * Assignment: Lab #9 Exercise #1
 * Exercise Description: [speaker]
 *
 * I acknowledge all content contained herein, excluding template or example
 * code, is my own original work.
 */ 
#include <avr/io.h>
unsigned char button = 0x00;
void set_PWM(double frequency) {
	
	
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

unsigned char sound = 0x00;
enum {init, off, wait_on, on, wait_off}p_state;
void power()
{
	switch(p_state)
	{
		case init:
		p_state = off;
		break;
		case off:
		if(button == 0x01)
		{
			p_state = wait_on;
		}
		else
		{
			p_state = off;
		}
		break;
		case wait_on:
		if(button == 0x01)
		{
			p_state = wait_on;
		}
		else
		{
			p_state = on;
		}
		break;
		case on:
		if(button == 0x01)
		{
			p_state = wait_off;
		}
		break;
		case wait_off:
		if(button == 0x01)
		{
			p_state = wait_off;
		}
		else
		{
			p_state = off;
		}
		break;
		default:
		p_state = off;
		break;
	}
	switch (p_state)
	{
		case off:
		sound = 0x00;
		break;
		case wait_on:
		sound = 0x00;
		break;
		case on:
		sound = 0x01;
		break;
		case wait_off:
		sound = 0x01;
		break;
		default:
		sound = 0x00;
		break;
	}
}

static char current_tone;
enum {init1, C, D, E, F, G, A, B, C5, wait_increase, wait_decrease } t_state;
void tune(){
	switch(t_state){
		case init1:
		t_state = C;
		current_tone = 0x01;
		break;
		case wait_increase:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if((button == 0x00) && (current_tone == 0x01))
		{
			t_state = D;
		}
		else if((button == 0x00) && (current_tone == 0x02))
		{
			t_state = E;
		}
		else if((button == 0x00) && (current_tone == 0x03))
		{
			t_state = F;
		}
		else if((button == 0x00) && (current_tone == 0x04))
		{
			t_state = G;
		}
		else if((button == 0x00) && (current_tone == 0x05))
		{
			t_state = A;
		}
		else if((button == 0x00) && (current_tone == 0x06))
		{
			t_state = B;
		}
		else if((button == 0x00) && (current_tone == 0x07))
		{
			t_state = C5;
		}
		else if((button == 0x00) && (current_tone == 0x08))
		{
			t_state = C5;
		}
		break;
		case wait_decrease:
		if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else if((button == 0x00) && (current_tone == 0x01))
		{
			t_state = C;
		}
		else if((button == 0x00) && (current_tone == 0x02))
		{
			t_state = C;
		}
		else if((button == 0x00) && (current_tone == 0x03))
		{
			t_state = D;
		}
		else if((button == 0x00) && (current_tone == 0x04))
		{
			t_state = E;
		}
		else if((button == 0x00) && (current_tone == 0x05))
		{
			t_state = F;
		}
		else if((button == 0x00) && (current_tone == 0x06))
		{
			t_state = G;
		}
		else if((button == 0x00) && (current_tone == 0x07))
		{
			t_state = A;
		}
		else if((button == 0x00) && (current_tone == 0x08))
		{
			t_state = B;
		}
		break;
		case C:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else
		{
			t_state = C;
		}
		break;
		case D:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = D;
		}
		break;
		case E:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = E;
		}
		break;
		case F:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = F;
		}
		break;
		case G:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = G;
		}
		break;
		case A:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = A;
		}
		break;
		case B:
		if(button == 0x02)
		{
			t_state = wait_increase;
		}
		else if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = B;
		}
		break;
		case C5:
		if(button == 0x04)
		{
			t_state = wait_decrease;
		}
		else
		{
			t_state = C5;
		}
		break;
		default:
		t_state = C;
		break;
	}
	switch(t_state)
	{
		case C:
		current_tone = 0x01;
		set_PWM(261.63);
		PORTB = 0x01;
		PORTC = 0x00;
		break;
		
		case D:
		current_tone = 0x02;
		set_PWM(293.66);
		PORTB = 0x02;
		PORTC = 0x00;
		break;
		
		case E:
		current_tone = 0x03;
		set_PWM(329.63);
		PORTB = 0x03;
		PORTC = 0x00;
		break;
		
		case F:
		current_tone = 0x04;
		set_PWM(349.23);
		PORTB = 0x04;
		PORTC = 0x00;
		break;
		
		case G:
		current_tone = 0x05;
		set_PWM(392);
		PORTB = 0x05;
		PORTC = 0x00;
		break;
		
		case A:
		current_tone = 0x06;
		set_PWM(440);
		PORTB = 0x06;
		PORTC = 0x00;
		break;
		
		case B:
		current_tone = 0x07;
		set_PWM(493.88);
		PORTB = 0x07;
		PORTC = 0x00;
		break;
		
		case C5:
		current_tone = 0x08;
		set_PWM(523.25);
		PORTB = 0x08;
		PORTC = 0x00;
		break;
		
		case wait_increase:
		PORTC = 0x01;
		break;
		
		case wait_decrease:
		PORTC = 0x02;
		break;
		
		default:
		current_tone = 0x00;
		set_PWM(0);
		PORTB = 0x00;
		PORTC = 0x00;
		break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	p_state = init;
	t_state = init1;
	
	PWM_on();
	set_PWM(0);
	sound = 0x00;
	
	while(1)
	{
		button = ~PINA & 0x07;
		power();
		if(sound)
		{
			tune();
		}
		else
		{
			set_PWM(0);
		}
	}
	PWM_off();
}