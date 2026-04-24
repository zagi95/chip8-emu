#include "cpu.h"

void fetch_instruction(uint8_t *memory, struct chip8_regset *cpu)
{
    cpu->operand = (memory[cpu->pc] << 8) | memory[cpu->pc + 1];
    cpu->pc += 2;
}

int16_t decode_instruction(uint16_t opcode)
{
    uint8_t high = opcode >> 12;
    uint8_t low  = opcode & 0x000F;
    
    switch (high) {
    case 0x0:
        if (opcode == 0x00E0) return OP_CLS;
        if (opcode == 0x00EE) return OP_RET;
        return OP_SYS;

    case 0x1: return OP_JP;           
    case 0x2: return OP_CALL;         
    case 0x3: return OP_SE_BYTE;      
    case 0x4: return OP_SNE_BYTE;     
    case 0x5: return OP_SE_REG;       
    case 0x6: return OP_LD_BYTE;      
    case 0x7: return OP_ADD_BYTE;     

    case 0x8:
        switch (low) {
        case 0x0: return OP_LD_REG;
        case 0x1: return OP_OR;
        case 0x2: return OP_AND;
        case 0x3: return OP_XOR;
        case 0x4: return OP_ADD_REG;
        case 0x5: return OP_SUB;
        case 0x6: return OP_SHR;
        case 0x7: return OP_SUBN;
        case 0xE: return OP_SHL;
        default:  return OP_INVALID;
        }

    case 0x9: return OP_SNE_REG;
    case 0xA: return OP_LD_I;
    case 0xB: return OP_JP_V0;
    case 0xC: return OP_RND;
    case 0xD: return OP_DRW;

    case 0xE:
        switch (opcode & 0x00FF) {
        case 0x9E: return OP_SKP;
        case 0xA1: return OP_SKNP;
        default:   return OP_INVALID;
        }

    case 0xF:
        switch (opcode & 0x00FF) {
        case 0x07: return OP_LD_DT;
        case 0x0A: return OP_LD_K;
        case 0x15: return OP_SET_DT;
        case 0x18: return OP_SET_ST;
        case 0x1E: return OP_ADD_I;
        case 0x29: return OP_LD_F;
        case 0x33: return OP_LD_BCD;
        case 0x55: return OP_STORE_REGS;
        case 0x65: return OP_LOAD_REGS;
        default:   return OP_INVALID;
        }

    default:
        return OP_INVALID;
    }
}
