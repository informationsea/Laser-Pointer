/*
 * AVR USART Module
 * Copyright (C) 2008-2012 Yasunobu Okamura All Rights Reserved
 */
#include "usart.h"


void usart_putc(const char data)
{
    loop_until_bit_is_set(UCSR0A,UDRE0);
    UDR0 = data; // Transmit
}

char usart_getc(void)
{
    loop_until_bit_is_set(UCSR0A,RXC0);
    return UDR0;
}

void usart_putstr(const char* str)
{
    while (*str) {
        usart_putc(*str++);
    }
}

#ifdef USART_SUPPORT_PROGMEM
void usart_putstr_P(PGM_P str)
{
    char ch;
    while((ch = pgm_read_byte(str++)) != '\0') {
        usart_putc(ch);
    }
}
#endif

#ifdef USART_GETSTR
size_t usart_getstr_s(char* str, size_t len)
{
    uint16_t i;
    for (i = 0; i < len - 1; i++) {
        char ch = usart_getc();
        switch (ch) {
        case '\r':
            goto finish;
        case 0x1b:
            str -= i;
            i = 0;
            goto finish;
        default:
            usart_putc(ch);
            *str++ = ch;
            break;
        }
    }
    finish: *str = 0;
    usart_putc('\r');
    usart_putc('\n');
    return i;
}

#endif

#ifdef USART_GETI
USART_INTEGER usart_geti()
{
    long i = 0;
#ifdef USART_SIGNED_SUPPORT
    int8_t sign = 1;
#endif
    while (1) {
        char ch = usart_getc();
        if ('0' <= ch && ch <= '9') { // only accept numbers
            i = i * 10 + (ch - '0');
            usart_putc(ch);
        } else if (ch == '\r') { // finish with return code
            usart_putc('\r');
            usart_putc('\n');
#ifdef USART_SIGNED_SUPPORT
            return i * sign;
#else
            return i;
#endif
#ifdef USART_SIGNED_SUPPORT
        } else if (ch == '-') { // minus number
            usart_putc(ch);
            sign = -1;
        } else if (ch == '+') { // plus number 
            usart_putc(ch);
            sign = +1;
#endif
        } else if (ch == 0x1b) { // cancel
            usart_putc('\r');
            usart_putc('\n');
            usart_putc('>');
            i = 0;
#ifdef USART_SIGNED_SUPPORT
            sign = 1;
#endif
        }
    }
    return 0;
}
#endif

#ifdef USART_PUTI
void usart_puti(USART_INTEGER n)
{
    char str[10]; // maximum 10 digits
    int i = 0, j;

#ifdef USART_SIGNED_SUPPORT
    if(n < 0){
        usart_putc('-');
        n = -n;
    }
#endif

    while (n > 0) { // calculate from the least significant digit
        str[i++] = n % 10 + '0';
        n /= 10;
    }

    for (j = i - 1; 0 <= j; j--) {// reverse output
        usart_putc(str[j]);
    }

    if (i == 0) {
        usart_putc('0');
    }
}
#endif

#ifdef USART_PUTHEX

void usart_puthex(const uint8_t ch)
{
    uint8_t most = (ch >> 4) & 0x0f;
    uint8_t least = ch & 0x0f;
    if (most < 10) {
        usart_putc('0' + most);
    } else {
        usart_putc('A' + most - 10);
    }
    if (least < 10) {
        usart_putc('0' + least);
    } else {
        usart_putc('A' + least - 10);
    }
}

#endif

