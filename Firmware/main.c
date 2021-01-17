#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

static void init(void)
{
	DDRB = 0b11111111;
}

int main()
{
	init();

	while (true)
	{
		PORTB = _BV(PB2);
		_delay_ms(1000);
		PORTB = _BV(PB3);
		_delay_ms(1000);
		PORTB = _BV(PB4);
		_delay_ms(1000);
	}
}
