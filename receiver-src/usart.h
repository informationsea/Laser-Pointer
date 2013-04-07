/*
 * USART Library for AVR Series
 * Copyright (C) 2008-2012 Yasunobu Okamura All Rights Reserved.
 */

#ifndef __USART_H__
#define __USART_H__

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

#ifdef UBRRH // Support Old AVR
#define UBRR0H UBRRH
#define UBRR0L UBRRL
#define UCSR0A UCSRA
#define U2X0   U2X
#define UCSR0B UCSRB
#define RXEN0  RXEN
#define TXEN0  TXEN
#define UDRE0  UDRE
#define UDR0   UDR
#define RXC0   RXC
#endif

#ifdef USART_COMPATIBLE
#include "usart_compatible.h"
#endif

/**
 * @brief Initialize USART
 * @parameter baud baud rate
 * @warning DO NOT pass variable as an argument.
 */
static inline void usart_init(uint32_t baud)
{
#ifdef ENABLE_DOUBLE_BOADRATE
    uint16_t ubrr = (unsigned int)((double)F_CPU / 8 / baud - 0.5);
#else
    uint16_t ubrr = (unsigned int)((double)F_CPU / 16 / baud - 0.5);
#endif

    UBRR0H = (uint8_t) (ubrr >> 8);
    UBRR0L = (uint8_t) ubrr;

#ifndef ENABLE_DOUBLE_BOADRATE
    UCSR0A = 0;
#else
    UCSR0A = _BV(U2X0);
#endif

    UCSR0B = _BV(RXEN0) | _BV(TXEN0); // Enable receiver and transmitter
}

/**
 * @brief Transmit one character
 * @param data one character
 */
void usart_putc(const char data);

/**
 * @brief Receive one character
 * @return a received character
 */
char usart_getc(void);

/**
 * @brief Transmit a string
 * @param str a string
 */
void usart_putstr(const char* str);

#ifdef USART_SUPPORT_PROGMEM
/**
 * @brief Transmit a string in the program memory
 * @param str a string in the program memory
 */
void usart_putstr_P(PGM_P str);
#endif

#ifdef USART_GETSTR
/**
 * @brief Receive a string
 * @param str a string buffer
 * @param len a length of the buffer
 */
size_t usart_getstr_s(char* str, size_t len);

/**
 * @brief Receive a string
 * @param str a string buffer
 */
#define usart_getstr(str) usart_getstr_s((str), sizeof(str))
#endif

#ifdef USART_SUPPORT_SIGNED
typedef long USART_INTEGER;
#else
typedef unsigned long USART_INTEGER;
#endif

#ifdef USART_GETI
/**
 * @brief Receive a long integer in human readable character
 * @return a long integer
 * @note if your want to use signed integer, you have to set USART_SUPPORT_SIGNED
 */
USART_INTEGER usart_geti(void);
#endif

#ifdef USART_PUTI
/**
 * @brief Transmit a long integer in human readable character
 * @param num a long integer
 * @note if your want to use signed integer, you have to set USART_SUPPORT_SIGNED
 */
void usart_puti(USART_INTEGER num);
#endif

#ifdef USART_PUTHEX
/**
 * @brief Transmit a byte integer in hexadecimal string
 * @param ch a byte integer
 */
void usart_puthex(const uint8_t ch);
#endif

#endif
