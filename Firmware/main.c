#include <stddef.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#define RED   OCR0A	// PB2
#define GREEN OCR1A	// PB3
#define BLUE  OCR1B	// PB4
#define BTN1  PD5	// Turn LEDs on
#define BTN2  PB0	// Switch colors

#define BIT_GET(x, b)	((x) & _BV(b))
#define BIT_SET(x, y)	((x) |= _BV(y))
#define BIT_CLR(x, y)	((x) &= ~_BV(y))
#define BTN1_PRESSED	(!BIT_GET(PIND, BTN1))
#define BTN2_PRESSED	(!BIT_GET(PINB, BTN2))

typedef struct
{
	uint8_t r, g, b;
} color_t;

static const color_t palette[] =
{
	{ 255, 0,   0   },	// Red
	{ 255, 128, 0   },	// Orange
	{ 255, 255, 0   },	// Yellow
	{ 0,   255, 0   },	// Green
	{ 0,   255, 255 },	// Cyan
	{ 0,   128, 255 },	// Azure
	{ 0,   0,   255 },	// Blue
	{ 128, 0,   255 },	// Violet
	{ 255, 0,   255 },	// Magenta
	{ 255, 0,   128 }	// Rose
};

#define NUM_COLORS	(sizeof(palette) / sizeof(palette[0]))

static void switch_color()
{
	static size_t color_idx;

	color_t c = palette[color_idx];
	RED   = c.r;
	GREEN = c.g;
	BLUE  = c.b;

	color_idx = (color_idx + 1) % NUM_COLORS;
}

static void switch_leds(bool on)
{
	if (on)
	{
		// Enable PWMs
		BIT_SET(TCCR0A, COM0A1);
		BIT_SET(TCCR1A, COM1A1);
		BIT_SET(TCCR1A, COM1B1);
	}
	else
	{
		// Disable PWMs
		BIT_CLR(TCCR0A, COM0A1);
		BIT_CLR(TCCR1A, COM1A1);
		BIT_CLR(TCCR1A, COM1B1);

		// Zero out output values
		PORTB = 0;
	}
}

static void init(void)
{
	// PB0 - Input, PB2-PB4 - Output
	DDRB  = 0b00011100;

	// Timer/Counter0: Fast PWM mode, Top = 0xFF, No prescaling
	TCCR0A = _BV(WGM01) | _BV(WGM00);
	TCCR0B = _BV(CS00);

	// Timer/Counter1: Fast PWM mode (8 bit), Top = 0xFF, No prescaling
	TCCR1A = _BV(WGM12) | _BV(WGM10);
	TCCR1B = _BV(CS10);

	TIFR  = _BV(TOV0);	// Clear Timer/Counter0 Overflow Flag
	TIMSK = _BV(TOIE0);	// Timer/Counter0 Overflow Interrupt Enable

	switch_color();
	switch_leds(false);

	sei();	// Enable interrupts
}

// Timer/Counter0 Overflow Interrupt (2 kHz)
ISR(TIMER0_OVF_vect)
{
	if (BTN2_PRESSED)
		switch_color();

	switch_leds(BTN1_PRESSED);
}

int main()
{
	init();

	while (true)
		sleep_mode();
}
