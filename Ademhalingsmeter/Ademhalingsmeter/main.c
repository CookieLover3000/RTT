/*
 * Ademhalingsmeter.c
 *
 * Created: 5-5-2022 10:49:36
 * Authors : Iwan & David
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void initADC();
void initUsart();
void writeChar(char x);
void writeString(char st[]);
void writeInt(int i);
uint16_t leesADCwaarde();
void printUsart(uint16_t a);

int main(void)
{
	initADC();
	initUsart();
	uint16_t adcWaarde;
	uint16_t adcWaardeTotaal = 0;
	
    while (1) 
    {
		for (int i = 0; i < 15; i++) {
			adcWaarde = leesADCwaarde();
			adcWaardeTotaal += adcWaarde;
		}
		
		adcWaardeTotaal /= 15;
		//adcWaardeTotaal -= ;
		
		//adcWaarde *= 1.5;
		//adcWaarde /= 10;
		//adcWaarde -= 20;
		//adcWaarde *= 1.5;
	
		if(adcWaardeTotaal > 250)
		adcWaardeTotaal = 0;
		if(adcWaardeTotaal < 0)
		adcWaardeTotaal = 0;
	
		
		printUsart(adcWaardeTotaal);
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
//	a %= 10; // werkt niet maar was wel fijner om te lezen omdat de schaal kleiner was.
	return a;
}

void printUsart(uint16_t x)
{
	writeInt(x);
	writeString("\n\r");
	_delay_ms(250);	
}

