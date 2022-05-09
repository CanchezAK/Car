/*
 * Car_Control.c
 *
 * Created: 21.04.2022 19:24:16
 * Author : CanchezAK
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CHECK_HAND1 (PIND & (1<<3))		//PD3, check to logic 1
#define CHECK_HAND0 !(PIND & (1<<3))	//PD3, check to logic 0
#define CHECK_POS1 (PIND & (1<<4))		//PD4, check to logic 1
#define CHECK_POS0 !(PIND & (1<<4))		//PD4, check to logic 0
#define CHECK_WASH1 (PIND & (1<<5))		//PD5, check to logic 1
#define CHECK_WASH0 !(PIND & (1<<5))	//PD5, check to logic 0
#define ControlRelayActive PORTD |= (1<<6)		//Set PD6
#define ControlRelayInactive PORTD &= ~(1<<6)		//Reset PD6
//Global variables
unsigned short tim = 0;
unsigned short tim_wash = 0;
unsigned short adc_data = 0;

void timer_init()
{
	
	TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);		//Divider 1024, Clear To Compare mode with OCR1A
	OCR1A = 1562;
	TIMSK1 |= (1<<OCIE1A);		//Output Compare A Match Interrupt Enable
}

ISR (TIMER1_COMPA_vect)
{	
	if (CHECK_HAND1)
	{
		while(CHECK_HAND1)
		{
			tim++;
			if (tim >= 300)
			{
				ControlRelayActive;
				while (CHECK_POS0);
				while (CHECK_POS1);
				ControlRelayInactive;
				_delay_ms(5000);
				
				if (CHECK_WASH1)
				{
					while (CHECK_WASH1)
					{
						tim_wash++;
						if (tim_wash >= 300)
						{
							ControlRelayActive;
							while (CHECK_POS0);
							while (CHECK_POS1);
							ControlRelayInactive;
						}
					}
					tim_wash = 0;
				}
			}
		}
		tim = 0;
	}
	if (CHECK_WASH1)
	{
		while (CHECK_WASH1)
		{
			tim_wash++;
			if (tim_wash >= 300)
			{
				for (uint8_t count = 0; count < 2; count++)
				{
					ControlRelayActive;
					while (CHECK_POS0);
					while (CHECK_POS1);
					ControlRelayInactive;
				}
			}
		}
		tim_wash = 0;
	}
	tim = 0;		//Delete counters
	tim_wash = 0;
}

void ADC_init()
{
	ADMUX |= (1<<REFS0) | (0<<ADLAR);
	ADCSRA |= (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);
	//Read data from ADCL & ADCH (I use ADCH only)
}

unsigned short ADC_ReadData()
{
	ADCSRA |= (1<<ADSC);
	while((ADCSRA & (1<<ADSC)));
	ADCSRA &= ~(1<<ADSC);
	return ADC;
}

int main(void)
{
	DDRD = 0x00;		//Clear PORTD parameters
	DDRD |= (1<<6) | (1<<7);		//PD6 & PD7 output
	PORTD = 0x00;		//Pull-Up to inputs
	PORTD &= ~(1<<6) & ~(1<<7);		//Low level to output PD6 & PD7
	if(CHECK_POS1)
	{
		while (CHECK_POS1)
		{
			ControlRelayActive;
		}
		ControlRelayInactive;
	}
	ADC_init();
	cli();
    timer_init();
	sei();
    while (1) 
    {
			adc_data = ADC_ReadData();
			if (adc_data < 557)
			{
				PORTD |= (1<<7);
			}
			else if (adc_data > 573)
			{
				PORTD &= ~(1<<7);
			}
    }
}

