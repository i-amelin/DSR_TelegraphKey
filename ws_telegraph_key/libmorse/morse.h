#ifndef MORSE_H
#define MORSE_H

#include <stdint.h>

typedef struct morse_symbol
{
    char symbol;
    uint8_t key;
} morse_symbol_t;

#define ABC_LENGTH 36

void init_symbol(morse_symbol_t *symbol);
void add_dot(morse_symbol_t *symbol);
void add_dash(morse_symbol_t *symbol);
void set_char(morse_symbol_t *symbol);

#endif /* MORSE_H */
