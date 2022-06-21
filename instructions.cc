#include<iostream>
#include "instructions.hpp"
#include <assert.h>
#include <bitset>

#define SIGN_EXTEND_12(x) ((x & 0xFFF) << 11 >> 11)

using namespace std;


int gibi(int ins, int s, int e) {
    return (ins >> e) & ((1 << ( s - e + 1)) -1);
}

void jal(int ins) {
    int imm = gibi(ins, 31, 12);
    int offset = (gibi(ins, 31, 30) << 20) | (gibi(ins, 30, 21)<<1) | (gibi(ins, 21, 20))<<11 | gibi(ins, 19, 12) <<12;
    regs[PC] += offset;
}

void imm(int ins) {
    int rd = gibi(ins, 11, 7);
    int funct3 = gibi(ins, 14, 12);
    int rs1 = gibi(ins, 19, 15);
    int imm = gibi(ins, 31, 20);
    switch(funct3) {
        case Funct3::ADDI:
            cout<<" ADDI";
            regs[rd] = regs[rs1] + imm;
            break;
        case Funct3::SLTI:
            cout<<" SLTI";
            regs[rd] = SIGN_EXTEND_12(imm) > regs[rs1] ? 1 : regs[rd];
            break;
        case Funct3::SLTIU:
            cout<<" SLTIU";
            regs[rd] = imm > regs[rs1] ? 1 : regs[rd];
            break;
        case Funct3::ANDI:
            cout<<" ANDI";
            regs[rd] = regs[rs1] & imm;
            break;
        case Funct3::ORI:
            cout<<" ORI";
            regs[rd] = regs[rs1] | imm;
            break;
        case Funct3::XORI:
            cout<<" XORI";
            regs[rd] = regs[rs1] ^ imm;
            break;
        case Funct3::SLLI:
            cout<<" SLLI";
            regs[rd] = regs[rs1] << (imm & 0x01f);
            break;
        case Funct3::SRLI:
            if (imm & 0b00100000) {
                cout<<" SRAI";
                regs[rd] = ( regs[rs1] >> (imm & 0x01f) );
            }else {
                cout<<" SRLI";
                regs[rd] = ((unsigned char) (regs[rs1])) >> (imm & 0x01f);
            }
            break;
        case Funct3::BRANCH:
            
        default:
            cout<<"---------ERROR--------------"<<endl;
    }
    cout<<endl;
}

void auipc(int ins) {
    int imm = gibi(ins, 31, 12);
    int rd = gibi(ins, 11, 7);
    int shift = imm << 12;
    regs[rd] = shift + regs[PC];
}

void op(int ins) {
    int funct7 = gibi(ins, 31, 25);
    int rs2 = gibi(ins, 24, 20);
    int rs1 = gibi(ins, 19, 15);
    int funct3 = gibi(ins, 14, 12);
    int rd = gibi(ins, 11, 7);
    /*
    switch(funct3) {
        case Funct3::ADDI:
            cout<<" ADDI";
            regs[rd] = regs[rs1] + imm;
            break;
        case Funct3::SLTI:
            cout<<" SLTI";
            regs[rd] = SIGN_EXTEND_12(imm) > regs[rs1] ? 1 : regs[rd];
            break;
        case Funct3::SLTIU:
            cout<<" SLTIU";
            regs[rd] = imm > regs[rs1] ? 1 : regs[rd];
            break;
        case Funct3::ANDI:
            cout<<" ANDI";
            regs[rd] = regs[rs1] & imm;
            break;
        case Funct3::OR:
            cout<<" OR";
            regs[rd] = regs[rs1] | imm;
            break;
        case Funct3::XOR:
            cout<<" XOR";
            regs[rd] = regs[rs1] ^ imm;
            break;
    }*/
}

void system_(int ins) {
    int funct12 = gibi(ins, 31, 20);
    int rs1 = gibi(ins, 19, 15);
    int funct3 = gibi(ins, 14, 12);
    int rd = gibi(ins, 11, 7);
}