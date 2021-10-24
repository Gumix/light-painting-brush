#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

#define RED   OCR0A	// PB2
#define GREEN OCR1A	// PB3
#define BLUE  OCR1B	// PB4

static void init(void)
{
	// PB0 - Input, PB2-PB4 - Output
	DDRB  = 0b00011100;

	// Timer/Counter0: Fast PWM mode, Top = 0xFF, No prescaling
	TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);

	// Timer/Counter1: Fast PWM mode (8 bit), Top = 0xFF, No prescaling
	TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM12) | _BV(WGM10);
	TCCR1B = _BV(CS10);
}

int main()
{
	init();

	while (true)
	{
		RED   = 255;
		GREEN = 64;
		BLUE  = 64;
		_delay_ms(1000);
		RED   = 64;
		GREEN = 255;
		BLUE  = 64;
		_delay_ms(1000);
		RED   = 64;
		GREEN = 64;
		BLUE  = 255;
		_delay_ms(1000);
	}
}
