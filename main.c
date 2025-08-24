/* EA7EE AtTiny XTAL Gen, based on mykhayloscherbak/si5351
GPL v2 LICENSE
*/

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include "registers-si5351.h"

#define SDA _BV(1)
#define SCL _BV(2)
#define ADDR (0x60)

static volatile uint8_t timerReady = 0;

ISR(TIM0_COMPA_vect) {
    timerReady = 0xff;
}

static void waitTimer(void) {
    cli();
    timerReady = 0;
    sei();
    while (0 == timerReady);
}

static inline void initGpioIdle(void) {
    DDRB &= ~(SDA|SCL);
    PUEB |= SDA|SCL;

    // ADC Configuration
    ADMUX = 0;
    DIDR0 = (1<<ADC0D);
    ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
}


static inline void SDADown(void) {
    PORTB &= ~(SDA);
    DDRB |= SDA;
    PUEB &= ~(SDA); 
}


static inline void SCLDown(void) {
    PORTB &= ~(SCL);
    DDRB |= SCL;
    PUEB &= ~(SCL);
}

static inline void SDAUp(void) {
    DDRB &= ~(SDA);
    PUEB |= SDA;
}

static inline void SCLUp(void) {
    DDRB &= ~(SCL);
    PUEB |= SCL;
}

static inline void initTim0(void){
    OCR0A = 159; /* slow */
    TCCR0B = _BV(CS01) | _BV(WGM02); /* no prescaler, CTC mode, | mode is not used because this is the only access*/
    TIMSK0 = _BV(OCIE0A);
}

static inline void deinitTim0(void) {
    TCCR0B = 0;
}

static inline void start(void) {
    initGpioIdle();
    waitTimer();
    SDADown();
    waitTimer();
    SCLDown();
}

static inline  void stop(void) {
    SDADown();
    waitTimer();
    SCLUp();
    waitTimer();
    SDAUp();
}

static void sendByte(const uint8_t _byte) {
    for (uint8_t bit = 0 ; bit < 8 ; bit++) {
        if ( 0 == (_byte & (0x80 >> bit))) SDADown();
        else SDAUp();
        waitTimer();
        SCLUp();
        waitTimer();
        SCLDown();
    }
    waitTimer();
    SDAUp();
    waitTimer();
    SCLUp();
    waitTimer();
    SCLDown();
}

static inline void sendRegData(void) {
    for (uint8_t reg = 0; reg < SI5351A_REVB_REG_CONFIG_NUM_REGS; reg++) {
        start();
        waitTimer();
        sendByte(ADDR << 1);
        sendByte(si5351a_revb_registers[reg].address);
        sendByte(si5351a_revb_registers[reg].value);
        stop();
        waitTimer();
    }
}

static inline void si5351_write(uint8_t reg_addr, uint8_t reg_value) {
    start();
    waitTimer();
    sendByte(ADDR << 1);
    sendByte(reg_addr);
    sendByte(reg_value);  	 
    stop();
    waitTimer();
} 

#define CFACTOR 1048575UL
#define FACTOR_B 1006031UL
#define FACTOR_A 42

static inline void sendAdjustData(uint8_t adjust) {
    int16_t centered = ((int16_t)adjust) - 128;  // -128..+127

    // Ajuste fino proporcional
    uint32_t b = FACTOR_B + ((uint32_t)centered<<6);
    uint32_t p1  = 128 * FACTOR_A + (unsigned long) (128 * b / CFACTOR) - 512;
    uint32_t p2 = 128 * b - CFACTOR * (unsigned long) (128 * b / CFACTOR);

    si5351_write(0x2C, (p1 & 0x00030000) >> 16);    
    si5351_write(0x2D, (p1 & 0x0000FF00) >> 8);
    si5351_write(0x2E, (p1 & 0x000000FF));
    si5351_write(0x2F, 0xF0 | ((p2 & 0x000F0000) >> 16));
    si5351_write(0x30, (p2 & 0x0000FF00) >> 8);
    si5351_write(0x31, (p2 & 0x000000FF));
}

static inline void waitms(const uint16_t _ms) {
    for (uint32_t cycle = 0; cycle < _ms * 50;cycle++) waitTimer();
}

volatile uint8_t ant_val=255;
volatile uint8_t adc_val=0;

int main(void) {
	// Internal 8MHz, prescaler 1
	CCP    = 0xD8;
	CLKMSR = 0x00;
	CCP    = 0xD8;
	CLKPSR = 0X00;

    initTim0();
    sei();
    waitms(10);
    sendRegData();
    while(1) {
        ADCSRA |= (1<<ADSC);
        while(ADCSRA & (1<<ADSC));
        adc_val = ADCL;
        if (ant_val != adc_val) {
            waitms(50);
            sendAdjustData(adc_val);
            ant_val = adc_val;
        }        
    }

}
