/*
 * Ademhalingsmeter.c
 *
 * Created: 5-5-2022 10:49:36
 * Authors : RTT Groep D3.2
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void initADC();
void initUsart();
void writeChar(char x);
void writeString(char st[]);
void writeInt(int i);
uint16_t leesADCwaarde();
void printUsart(int x);

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
	initADC();
	initUsart();
	
	
const float VCC = 4.98;
const float R_DIV = 47500.0;
const float STRAIGHT_RESISTANCE = 37300.0;
const float BEND_RESISTANCE = 90000.0;

while (1) {
	
	int flexADC = leesADCwaarde();
	float flexV = flexADC * VCC / 1023.0;
	float flexR = R_DIV * (VCC / flexV - 1.0);

	// Use the calculated resistance to estimate the sensor's bend angle:

	float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE, 0, 90.0);
	
	
	writeString("Bend: ");
	printUsart(angle);
}	
	return 0;
}

void initADC()
{
	// Welke Voltage reference nodig (table 23-3)
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2); // Voltage Reference: AVcc witch external capacitator at AREF pin - Analog Channel = ADC5
	ADCSRA = (1 << ADEN) | (1 << ADPS0) | (1 << ADPS2); // Prescaler 32
}

void initUsart()
{
	UCSR0A = 0;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); /* Enable de USART Transmitter */
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8 data bits, 1 stop bit */
	UBRR0H=00;
	UBRR0L=103; /* baudrade 9600 */
}

void writeChar(char x)
{
	while (~UCSR0A & (1 << UDRE0));
	UDR0 = x;
}

void writeString(char st[])
{
	for(uint8_t i = 0 ; st[i] != 0 ; i++)
	writeChar(st[i]);
}

void writeInt(int i)
{
	char buffer[8];
	itoa(i,buffer,10);
	writeString(buffer);
}

uint16_t leesADCwaarde()
{
	ADCSRA |= (1 << ADSC | 1 << ADIF); /* start ADC conversion */
	while(~ADCSRA & (1 << ADIF));
	uint16_t a = ADC;
	return a;
}

void printUsart(int x)
{
	writeInt(x);
	writeString("\n\r");
	_delay_ms(250);	
}

