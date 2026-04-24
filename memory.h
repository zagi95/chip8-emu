#include "stdint.h"
#include <stddef.h>



int memory_init(uint8_t **memory, int size);
uint8_t memory_read(uint8_t *memory, int address);
void memory_write(uint8_t *memory, uint16_t address, uint8_t value);
void memory_free(uint8_t **memory);
int get_sprite_address(int sprite);
