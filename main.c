#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"
#include "memory.h"
#include "time.h"
#include <stdint.h>
#include "framebuffer.h"
#include "cpu.h"
#include "display.h"

void parse_program(uint8_t *program_ascii, size_t program_size, uint8_t *memory)
{
    size_t pos = 0;
    for(size_t i = 0; i < program_size; i+=2) {
        pos = i / 2;
        switch(program_ascii[i]) {
        case 0x30:
            memory[0x200 + pos] |= (0x0 << 4);
            break;
        case 0x31:
            memory[0x200 + pos] |= (0x1 << 4);
            break;
        case 0x32:
            memory[0x200 + pos] |= (0x2 << 4);
            break;
        case 0x33:
            memory[0x200 + pos] |= (0x3 << 4);
            break;
        case 0x34:
            memory[0x200 + pos] |= (0x4 << 4);
            break;
        case 0x35:
            memory[0x200 + pos] |= (0x5 << 4);
            break;
        case 0x36:
            memory[0x200 + pos] |= (0x6 << 4);
            break;
        case 0x37:
            memory[0x200 + pos] |= (0x7 << 4);
            break;
        case 0x38:
            memory[0x200 + pos] |= (0x8 << 4);
            break;
        case 0x39:
            memory[0x200 + pos] |= (0x9 << 4);
            break;
        case 0x41:
            memory[0x200 + pos] |= (0xA << 4);
            break;
        case 0x42:
            memory[0x200 + pos] |= (0xB << 4);
            break;
        case 0x43:
            memory[0x200 + pos] |= (0xC << 4);
            break;
        case 0x44:
            memory[0x200 + pos] |= (0xD << 4);
            break;
        case 0x45:
            memory[0x200 + pos] |= (0xE << 4);
            break;
        case 0x46:
            memory[0x200 + pos] |= (0xF << 4);
            break;
        }
        switch(program_ascii[i+1]) {
        case 0x30:
            memory[0x200 + pos] |= 0x0;
            break;
        case 0x31:
            memory[0x200 + pos] |= 0x1;
            break;
        case 0x32:
            memory[0x200 + pos] |= 0x2;
            break;
        case 0x33:
            memory[0x200 + pos] |= 0x3;
            break;
        case 0x34:
            memory[0x200 + pos] |= 0x4;
            break;
        case 0x35:
            memory[0x200 + pos] |= 0x5;
            break;
        case 0x36:
            memory[0x200 + pos] |= 0x6;
            break;
        case 0x37:
            memory[0x200 + pos] |= 0x7;
            break;
        case 0x38:
            memory[0x200 + pos] |= 0x8;
            break;
        case 0x39:
            memory[0x200 + pos] |= 0x9;
            break;
        case 0x41:
            memory[0x200 + pos] |= 0xA;
            break;
        case 0x42:
            memory[0x200 + pos] |= 0xB;
            break;
        case 0x43:
            memory[0x200 + pos] |= 0xC;
            break;
        case 0x44:
            memory[0x200 + pos] |= 0xD;
            break;
        case 0x45:
            memory[0x200 + pos] |= 0xE;
            break;
        case 0x46:
            memory[0x200 + pos] |= 0xF;
            break;
        }
    }
}

void load_program(uint8_t *memory, char *path)
{
    FILE *program = fopen(path, "rb");
    uint8_t *program_ascii = NULL;
    if(!program) {
        exit(-1);
    }

    fseek(program, 0, SEEK_END);
    long program_size = ftell(program);
    rewind(program);
    
    if((program_size < 0) || program_size > (0xFFF - 0x200 + 1)) {
        fclose(program);
        exit(-1);
    }

    program_ascii = malloc(program_size * sizeof(uint8_t));
    size_t cnt = fread(program_ascii, 1, program_size, program);

    parse_program(program_ascii, program_size, memory);
    
    //size_t cnt2 = fread(memory + 0x200, 1, program_size, program);
    //if(cnt2 != (size_t)program_size) {
    //    fclose(program);
    //    exit(-1);
    //}
    
    fclose(program);
}

void execute_cycle(uint8_t *memory, struct chip8_regset *cpu, uint16_t *stack, uint8_t *keypad) 
{
    fetch_instruction(memory, cpu);
    int16_t decoded = decode_instruction(cpu->operand);
    struct chip8_op_info op_info;
    op_info.x = (cpu->operand >> 8) & 0x0F;
    op_info.y = (cpu->operand >> 4) & 0x0F;
    op_info.kk = cpu->operand & 0x00FF;
    op_info.nnn = cpu->operand & 0x0FFF;
    op_info.n = cpu->operand & 0x000F;
    
    switch(decoded) {
    case OP_CLS:
        for(int i = 0; i < 64*32; i++) {
            framebuffer[i] = 0;
        }
        break;
    case OP_RET:
        cpu->pc = stack[cpu->sp];
        cpu->sp -= 1;
        break;
    case OP_SYS:
        fprintf(stderr, "OP_SYS ignored on modern interpreters!\n");
        exit(-1);
        break;
    case OP_JP:
        cpu->pc = op_info.nnn;
        break;
    case OP_CALL:
        stack[cpu->sp++] = cpu->pc;
        cpu->pc = op_info.nnn;
        break;
    case OP_SE_BYTE:
        if(cpu->v[op_info.x] == op_info.kk) {
            cpu->pc += 2;
        }
        break;
    case OP_SNE_BYTE:
        if(cpu->v[op_info.x] != op_info.kk) {
            cpu->pc += 2;
        }
        break;
    case OP_SE_REG:
        if(cpu->v[op_info.x] == cpu->v[op_info.y]) {
            cpu->pc += 2;
        }
        break;
    case OP_LD_BYTE:
        cpu->v[op_info.x] = op_info.kk;
        break;
    case OP_ADD_BYTE:
        cpu->v[op_info.x] += op_info.kk;
        break;
    case OP_LD_REG:
        cpu->v[op_info.x] = cpu->v[op_info.y];
        break;
    case OP_OR:
        cpu->v[op_info.x] = cpu->v[op_info.x] | cpu->v[op_info.y];
        break;
    case OP_AND:
        cpu->v[op_info.x] = cpu->v[op_info.x] & cpu->v[op_info.y];
        break;
    case OP_XOR:
        cpu->v[op_info.x] = cpu->v[op_info.x] ^ cpu->v[op_info.y];
        break;
    case OP_ADD_REG:
        cpu->v[op_info.x] = cpu->v[op_info.x] + cpu->v[op_info.y];
        cpu->v[0xf] = 1;
        break;
    case OP_SUB:
        if(cpu->v[op_info.x] > cpu->v[op_info.y]) {
            cpu->v[0xF] = 1;
        }
        else {
            cpu->v[0xF] = 0;
        }
        cpu->v[op_info.x] = cpu->v[op_info.x] - cpu->v[op_info.y];
        break;
    case OP_SHR:
        cpu->v[0xF] = cpu->v[op_info.x] & 0x1;
        cpu->v[op_info.x] = cpu->v[op_info.x] >> 1;
        break;
    case OP_SUBN:
        cpu->v[0xF] = cpu->v[op_info.y] > cpu->v[op_info.x] ? 1 : 0;
        cpu->v[op_info.x] = cpu->v[op_info.y] - cpu->v[op_info.x];
        break;
    case OP_SHL:
        cpu->v[0xF] = (cpu->v[op_info.x] & 0x8000) ? 1 : 0;
        cpu->v[op_info.x] *= 2;
        break;
    case OP_SNE_REG:
        if (cpu->v[op_info.x] != cpu->v[op_info.y]) {
            cpu->pc += 2;
        }
        break;
    case OP_LD_I:
        cpu->i = op_info.nnn;
        break;
    case OP_JP_V0:
        cpu->pc = op_info.nnn + cpu->v[0x0];
        break;
    case OP_RND:
        cpu->v[op_info.x] = (rand() % 256) & op_info.kk;
        break;
    case OP_DRW:
        framebuffer_update(cpu, memory, op_info);
        break;
    case OP_SKP:
        if(keypad[cpu->v[op_info.x]]) {
            cpu->pc += 2;
        }
        break;
    case OP_SKNP:
        if(!keypad[cpu->v[op_info.x]]) {
            cpu->pc += 2;
        }
        break;
    case OP_LD_DT:
        cpu->v[op_info.x] = cpu->delay_timer;
        break;
    case OP_LD_K:
        ;
        int key = 0xFFFF;
        while(key == 0xFFFF) {
            for(int i = 0; i < 16; i++) {
                if(keypad[i] == 1) {
                    key = i;
                    break;
                }
            }
            display_handle_events(keypad);
        }
        cpu->v[op_info.x] = key;
        break;
    case OP_SET_DT:
        cpu->delay_timer = cpu->v[op_info.x];
        break;
    case OP_SET_ST:
        cpu->sound_timer = cpu->v[op_info.x];
        break;
    case OP_ADD_I:
        cpu->i += cpu->v[op_info.x]; 
        break;
    case OP_LD_F:
        cpu->i = get_sprite_address(cpu->v[op_info.x]);
        break;
    case OP_LD_BCD:
        
        break;
    default:
        break;
    }
}

static char *program_path = "/home/zagi/projects/chip8-emu/program.ch8";

int main(void)
{
    srand(time(NULL));
    struct chip8_regset cpu;
    uint8_t *memory = NULL;
    uint16_t stack[0x10];
    uint8_t keypad[0xF];
    
    display_init();
    
    if(!memory_init(&memory, 0x1000)) {
        exit(-1);
    }

    load_program(memory, program_path);
    cpu.pc = 0x200;
    
    int running = 1;
    while (running) {
        running = display_handle_events(keypad);
        execute_cycle(memory, &cpu,
                      stack, keypad); // in case of draw updated framebuffer
        if (draw_flag) {
            display_update(framebuffer);
            draw_flag = 0;
        }
        
    }
    
    memory_free(&memory);
    display_close();
}
