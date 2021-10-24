#include <stddef.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

#define RED   OCR0A	// PB2
#define GREEN OCR1A	// PB3
#define BLUE  OCR1B	// PB4

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
		switch_color();
		_delay_ms(100);
	}
}
