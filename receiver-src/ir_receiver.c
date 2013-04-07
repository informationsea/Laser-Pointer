#include "ir_receiver.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "usart.h"

volatile unsigned char ir_last_command;

static volatile enum {IDLE,
                      FIRST_RECEIVE,
                      FIRST_BLACK,
                      SECOND_RECEIVE
} ir_state;

#define T1_TOLERANCE 3

#define T1_FIRST_RECEIVE_MS           148
#define T1_BLACK_MS                   15
#define T1_SECOND_RECEIVE_MS_NEXT     59
#define T1_SECOND_RECEIVE_MS_PREVIOUS 29

#define IF_MS(ms, tcnt) if (ms - T1_TOLERANCE < tcnt && tcnt < ms + T1_TOLERANCE)

void ir_init(void)
{
    //usart_init(38400);
    //usart_putstr("Initialized\r\n");
    
    DDRD   &= ~_BV(PD4);
    PCMSK2 |= _BV(PCINT20);
    PCICR  |= _BV(PCIE2);
    ir_last_command = IR_COMMAND_NONE;
    ir_state = IDLE;
    TCCR1B = _BV(CS12) | _BV(CS10); // 1024 prescaler // 1ms = 11.7
}

ISR(PCINT2_vect)
{
    unsigned int tcnt = TCNT1;

    if ((PIND & _BV(PD4)) == 0) {
        PORTB |= _BV(2);
        
        switch (ir_state) {
        case IDLE:
            TCNT1 = 0;
            ir_state = FIRST_RECEIVE;
            break;
        case FIRST_BLACK:
            IF_MS(T1_BLACK_MS, tcnt) {
                ir_state = SECOND_RECEIVE;
                TCNT1 = 0;
            } else {
                ir_state = IDLE;
            }
            break;
        default:
            ir_state = IDLE;
            break;
        }

    } else {
        PORTB &= ~_BV(2);

        switch (ir_state) {
        case FIRST_RECEIVE:
            IF_MS(T1_FIRST_RECEIVE_MS, tcnt) {
                ir_state = FIRST_BLACK;
                TCNT1 = 0;
            } else {
                ir_state = IDLE;
            }

            break;
        case SECOND_RECEIVE:
            IF_MS(T1_SECOND_RECEIVE_MS_NEXT, tcnt) {
                ir_last_command = IR_COMMAND_NEXT;
            }
            IF_MS(T1_SECOND_RECEIVE_MS_PREVIOUS, tcnt) {
                ir_last_command = IR_COMMAND_PREVIOUS;
            }            
            ir_state = IDLE;

//            usart_putstr("IR TCNT ");
//            usart_puti(tcnt);
//            usart_putstr("\r\n");
            break;
        default:
            ir_state = IDLE;
            break;
        }
    }
}

