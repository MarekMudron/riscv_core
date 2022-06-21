
#define MEM_SIZE 0x80000

enum Ops {
    LUI = 0b0110111,    // load upper intermediate
    LOAD = 0b0000011,
    STORE = 0b0100011,

    AUIPC = 0b0010111,  // add upper intermediate to PC
    JAL = 0b1101111,    
    JALR = 0b1100111,
    BRANCH = 0b1100011,
    
    IMM = 0b0010011,
    OP = 0b0110011,

    MISC = 0b0001111,
    SYSTEM = 0b1110011
};

enum Funct3 {
    ADDI = 0b000,
    ADD = 0b000,
    SUB = 0b000,

    SLLI = 0b001,
    SRLI = 0b101,
    SLTI = 0b010,
    SLT = 0b010,
    SLTIU =0b011,
    SLTU = 0b011,

    XORI = 0b100,
    XOR = 0b100,
    ORI = 0b110,
    OR = 0b110,
    ANDI = 0b111,
    AND = 0b111,
    
    SLL = 0b001,
    SRL = 0b101,
    SRLT = 0b101,
    SRA = 0b101
};

extern int regs [33];
extern int PC;
extern char memory[MEM_SIZE];

int gibi(int ins, int s, int e);

void jal(int ins);

void imm(int ins);

void auipc(int ins);

void system_(int ins);

void op(int ins);