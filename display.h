#include "stdint.h"
#include <stdint.h>

void display_init(void);
void display_update(const uint8_t *framebuffer);
void display_close(void);
int display_handle_events(uint8_t *key);


/*
 * tipke
 *
 * 1    2	3	C
 * 4	5	6	D
 * 7	8	9	E
 * A	0	B	F
 *
 */
