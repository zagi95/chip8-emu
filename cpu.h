#include <stdint.h>

struct chip8_regset {
    uint8_t v[0x10];
    uint16_t i;
    uint16_t sp;
    uint8_t sound_timer;
    uint8_t delay_timer;
    uint16_t pc;
    uint16_t operand;
};

enum chip8_opcode {
    OP_INVALID = -1,
    // 0x0000 group
    OP_CLS,         // 00E0 - Clear screen
    OP_RET,         // 00EE - Return from subroutine
    OP_SYS,         // 0nnn - Jump to machine code routine (not used on modern interpreters)

    // 1nnn - 2nnn
    OP_JP,          // 1nnn - Jump to address
    OP_CALL,        // 2nnn - Call subroutine

    // 3xkk - 7xkk
    OP_SE_BYTE,     // 3xkk - Skip next if Vx == kk
    OP_SNE_BYTE,    // 4xkk - Skip next if Vx != kk
    OP_SE_REG,      // 5xy0 - Skip next if Vx == Vy
    OP_LD_BYTE,     // 6xkk - Load Vx with byte kk
    OP_ADD_BYTE,    // 7xkk - Add kk to Vx (no carry)

    // 8xy0 - 8xyE (arithmetic/logical between registers)
    OP_LD_REG,      // 8xy0 - Vx = Vy
    OP_OR,          // 8xy1 - Vx = Vx | Vy
    OP_AND,         // 8xy2 - Vx = Vx & Vy
    OP_XOR,         // 8xy3 - Vx = Vx ^ Vy
    OP_ADD_REG,     // 8xy4 - Vx = Vx + Vy (VF = carry)
    OP_SUB,         // 8xy5 - Vx = Vx - Vy (VF = not borrow)
    OP_SHR,         // 8xy6 - Vx = Vy >> 1, VF = LSB(Vy) (original Chip‑8 shifts Vy)
    OP_SUBN,        // 8xy7 - Vx = Vy - Vx (VF = not borrow)
    OP_SHL,         // 8xyE - Vx = Vy << 1, VF = MSB(Vy)
    
    // 9xy0 - Annn
    OP_SNE_REG,     // 9xy0 - Skip next if Vx != Vy
    OP_LD_I,        // Annn - I = address nnn

    // Bnnn - Dxyn
    OP_JP_V0,       // Bnnn - Jump to nnn + V0
    OP_RND,         // Cxkk - Vx = random uint8_t & kk
    OP_DRW,         // Dxyn - Draw sprite at (Vx, Vy) with height n

    // Ex9E - ExA1 (keyboard)
    OP_SKP,         // Ex9E - Skip next if key with value Vx is pressed
    OP_SKNP,        // ExA1 - Skip next if key with value Vx is not pressed

    // Fx07 - Fx65 (timer, sound, BCD, memory)
    OP_LD_DT,       // Fx07 - Vx = delay timer
    OP_LD_K,        // Fx0A - Wait for key press, store in Vx
    OP_SET_DT,      // Fx15 - delay timer = Vx
    OP_SET_ST,      // Fx18 - sound timer = Vx
    OP_ADD_I,       // Fx1E - I = I + Vx
    OP_LD_F,        // Fx29 - I = address of hex digit sprite for Vx
    OP_LD_BCD,      // Fx33 - Store BCD of Vx at I, I+1, I+2
    OP_STORE_REGS,  // Fx55 - Store V0..Vx into memory starting at I
    OP_LOAD_REGS    // Fx65 - Load V0..Vx from memory starting at I
};

struct chip8_op_info {
    uint8_t x;
    uint8_t y;
    uint8_t kk;
    uint8_t n;
    uint16_t nnn;
};

void fetch_instruction(uint8_t *memory, struct chip8_regset *cpu);
int16_t decode_instruction(uint16_t opcode);
