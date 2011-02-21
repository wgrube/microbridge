/**
 *
 * Library for the max3421e USB host controller shield produced by circuitsathome and Sparkfun.
 * This is a low-level interface that provides access to the internal registers and polls the
 * controller for state changes.
 *
 * This library is based on work from Oleg, but has been ported to C and heavily restructured.
 * Control over the GPIO pins has been stripped.
 *
 * Note that the current incarnation of this library only supports the Arduino Mega with a
 * hardware mod to rewire the MISO, MOSI, and CLK SPI pins.
 *
 * http://www.circuitsathome.com/
 */
#ifndef _MAX3421E_H_
#define _MAX3421E_H_

#include <stdbool.h>
#include <stdint.h>

#include "../spi.h"
#include "max3421e_constants.h"

/**
 * Max3421e registers in host mode.
 */
enum
{
	MAX_REG_RCVFIFO = 0x08,
	MAX_REG_SNDFIFO = 0x10,
	MAX_REG_SUDFIFO = 0x20,
	MAX_REG_RCVBC = 0x30,
	MAX_REG_SNDBC = 0x38,
	MAX_REG_USBIRQ = 0x68,
	MAX_REG_USBIEN = 0x70,
	MAX_REG_CPUCTL = 0x80,
	MAX_REG_USBCTL = 0x78,
	MAX_REG_PINCTL = 0x88,
	MAX_REG_REVISION = 0x90,
	MAX_REG_FNADDR = 0x98,
	MAX_REG_GPINIRQ = 0xb0,
	MAX_REG_HIRQ = 0xc8,
	MAX_REG_HIEN = 0xd0,
	MAX_REG_MODE = 0xd8,
	MAX_REG_PERADDR = 0xe0,
	MAX_REG_HCTL = 0xe8,
	MAX_REG_HXFR = 0xf0,
	MAX_REG_HRSL = 0xf8
} max_registers;

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define MAX_SS(x) { if (x) PORTB |= 0x10; else PORTB &= ~0x10; }
#define MAX_INT() ((PORTH & 0x40) >> 0x40)
#define MAX_GPX() ((PORTH & 0x20) >> 0x20)
#define MAX_RESET(x) { if (x) PORTH |= 0x10; else PORTH &= ~0x10; }

#elif defined(__AVR_ATmega328P__)

#define MAX_SS(x) SPI_SS(x)
#define MAX_INT() ((PORTB & 2) >> 1)
#define MAX_GPX() (PORTB & 1)
#define MAX_RESET(x) { if (x) PORTD |= 0x80; else PORTD &= ~0x80; }

#endif

void max3421e_init();
void max3421e_write(uint8_t reg, uint8_t val);
uint8_t * max3421e_writeMultiple(uint8_t reg, uint8_t count, uint8_t * values);
void max3421e_gpioWr(uint8_t val);
uint8_t max3421e_read(uint8_t reg);
uint8_t * max3421e_readMultiple(uint8_t reg, uint8_t count, uint8_t * values);
uint8_t max3421e_gpioRd(void);
boolean max3421e_reset();
boolean max3421e_vbusPwr(boolean action);
void max3421e_busprobe(void);
void max3421e_powerOn();
uint8_t max3421e_getVbusState();

uint8_t max3421e_poll(void);

uint8_t max3421e_interruptHandler(void);
uint8_t max3421e_gpxInterruptHandler(void);

#endif //_MAX3421E_H_