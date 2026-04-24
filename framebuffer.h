#include "stdint.h"

struct chip8_op_info;
struct chip8_regset;

extern uint8_t framebuffer[64*32];
extern int draw_flag;

void framebuffer_update(struct chip8_regset *cpu, uint8_t *memory,
                        struct chip8_op_info opi);
