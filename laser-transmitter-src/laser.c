#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>


#define LASER_ON()  (PORTB |= _BV(3))
#define LASER_OFF() (PORTB &= ~_BV(3))

#define IR_ON()  (DDRB |= _BV(4))
#define IR_OFF() (DDRB &= ~_BV(4))

#define RAW_BUTTON_RED()    (PIND & _BV(1))
#define RAW_BUTTON_BLUE()   (PINA & _BV(0))
#define RAW_BUTTON_YELLOW() (PINA & _BV(1))

static volatile unsigned char button_red_count    = 0;
static volatile unsigned char button_blue_count   = 0;
static volatile unsigned char button_yellow_count = 0;

#define BUTTON_OK_COUNT  100
#define BUTTON_RED()    (button_red_count > BUTTON_OK_COUNT)
#define BUTTON_BLUE()   (button_blue_count > BUTTON_OK_COUNT)
#define BUTTON_YELLOW() (button_yellow_count > BUTTON_OK_COUNT)

int main(void)
{
    DDRB = ~_BV(4);
    DDRD = ~_BV(1);
    DDRA = 0;

    OCR0A = 70;
    TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
    TCCR0B = _BV(CS00);

    ICR1 = 211; // 8MHz/38kHz
    OCR1B = ICR1/3; // 1/3 duty
    TIMSK = _BV(ICIE1);
    TCCR1A = _BV(COM1B1) | _BV(WGM11);
    TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);

    sei();


    bool last_blue = false;
    bool last_yellow = false;
    
    while(1) {
        if (BUTTON_RED())
            LASER_ON();
        else
            LASER_OFF();

        bool current_blue   = BUTTON_BLUE();
        bool current_yellow = BUTTON_YELLOW();

        if (current_blue && current_blue != last_blue) {
            LASER_OFF();
            IR_ON();
            _delay_ms(10);
            IR_OFF();
            _delay_ms(1);
            IR_ON();
            _delay_ms(2);
            IR_OFF();
        }

        if (current_yellow && current_yellow != last_yellow) {
            LASER_OFF();
            IR_ON();
            _delay_ms(10);
            IR_OFF();
            _delay_ms(1);
            IR_ON();
            _delay_ms(4);
            IR_OFF();
        }

        last_blue   = current_blue;
        last_yellow = current_yellow;
    }
    return 0;
}

ISR(TIMER1_CAPT_vect)
{
    if (RAW_BUTTON_RED()) {
        if (button_red_count < 255)
            button_red_count += 1;
    } else {
        button_red_count = 0;
    }

    if (RAW_BUTTON_BLUE()) {
        if (button_blue_count < 255)
            button_blue_count += 1;
    } else {
        button_blue_count = 0;
    }

    if (RAW_BUTTON_YELLOW()) {
        if (button_yellow_count < 255)
            button_yellow_count += 1;
    } else {
        button_yellow_count = 0;
    }

}
