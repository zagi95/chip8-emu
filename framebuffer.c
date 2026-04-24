#include "framebuffer.h"
#include "cpu.h"
#include "stddef.h"

uint8_t framebuffer[64*32];
int draw_flag = 0;

void framebuffer_update(struct chip8_regset *cpu, uint8_t *memory,
                        struct chip8_op_info opi)
{
    uint8_t start_x = cpu->v[opi.x] % 64;
    uint8_t start_y = cpu->v[opi.y] % 32;
    cpu->v[0xF] = 0;

    for (unsigned short row = 0; row < opi.n; row++) {
        uint8_t sprite_byte = memory[cpu->i + row];
        uint8_t y_pos = (start_y + row) % 32;

        for (unsigned short col = 0; col < 8; col++) {
            if ((sprite_byte >> (7 - col)) & 1) {
                uint8_t x_pos = (start_x + col) % 64;
                size_t idx = y_pos * 64 + x_pos;

                if (framebuffer[idx] == 1) {
                    cpu->v[0xF] = 1;
                }
                framebuffer[idx] ^= 1;
            }
        }
    }
    draw_flag = 1;
}
