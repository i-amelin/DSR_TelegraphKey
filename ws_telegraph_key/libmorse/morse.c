#include "morse.h"

static const morse_symbol_t symbols[] =
{
    { 'A', 0b00000101 },
    { 'B', 0b00011000 },
    { 'C', 0b00011010 },
    { 'D', 0b00001100 },
    { 'E', 0b00000010 },
    { 'F', 0b00000110 },
    { 'G', 0b00001110 },
    { 'H', 0b00010000 },
    { 'I', 0b00000100 },
    { 'J', 0b00010111 },
    { 'K', 0b00001101 },
    { 'L', 0b00010100 },
    { 'M', 0b00000111 },
    { 'N', 0b00000110 },
    { 'O', 0b00001111 },
    { 'P', 0b00010110 },
    { 'Q', 0b00011101 },
    { 'R', 0b00001010 },
    { 'S', 0b00001000 },
    { 'T', 0b00000011 },
    { 'U', 0b00001001 },
    { 'V', 0b00010001 },
    { 'W', 0b00001011 },
    { 'X', 0b00011001 },
    { 'Y', 0b00011011 },
    { 'Z', 0b00011100 },
    
    { '1', 0b00101111 },
    { '2', 0b00100111 },
    { '3', 0b00100011 },
    { '4', 0b00100001 },
    { '5', 0b00100000 },
    { '6', 0b00110000 },
    { '7', 0b00111000 },
    { '8', 0b00111100 },
    { '9', 0b00111110 },
    { '0', 0b00111111 }
};

void init_symbol(morse_symbol_t *symbol)
{
    symbol->symbol = 0;
    symbol->key = 1;
}

void add_dot(morse_symbol_t *symbol)
{
	uint8_t key = symbol->key;
    key <<= 1;
    symbol->key = key;
}

void add_dash(morse_symbol_t *symbol)
{
    uint8_t key = symbol->key;
    key <<= 1;
    key |= 1;
    symbol->key = key;
}

void set_char(morse_symbol_t *symbol)
{
	int i = 0;
	int is_set = 0;

	while (i < ABC_LENGTH && !is_set)
	{
		uint8_t not_equals = symbols[i].key ^ (symbol->key);
		if (!not_equals)
		{
			symbol->symbol = symbols[i].symbol;
			is_set = 1;
		}
		i++;
	}

	if (!is_set)
	{
		symbol->symbol = ' ';
	}
}