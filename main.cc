#include<iostream>
#include<string.h>

using namespace std;

/*
31 general-purpose registers x1–x31, which hold integer values -> 32 bits
register x0 is hardwired to the constant 0
*/
int regs [32];

// program counter holds the address of the current instruction
int pc;

int main() {
    regs[0] = 0;


    return 0;
}