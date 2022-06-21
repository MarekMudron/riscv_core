#include<iostream>
#include<string.h>
#include <assert.h>
#include  <iomanip>
#include "ELFIO/elfio/elfio.hpp"
#include <experimental/filesystem>
#include "instructions.hpp"

namespace fs = std::experimental::filesystem;
using namespace std;
using namespace ELFIO;




/*
31 general-purpose registers x1–x31, which hold integer values -> 32 bits
register x0 is hardwired to the constant 0
*/
int regs [33];
int PC = 32;

// 64k of memory at 0x80000000
char memory[MEM_SIZE] = {0};

inline bool ends_with(std::string const & value, std::string const & ending)
{
    if (ending.size() > value.size()) return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

vector<string> getTests(string path) {
    bool isDump, isrv32;
    vector<string> paths;
    for (const auto & entry : fs::directory_iterator(path)) {
        string path = entry.path().generic_u8string();
        string fn = entry.path().filename().generic_u8string();
        isrv32 = fn.rfind("rv32ui-v-add") == 0;
        isDump = ends_with(fn, ".dump");
        if(!isDump && isrv32) {
            paths.push_back(path);
        }
    }
    return paths;
}

bool writeSegment(const char* dat, Elf_Xword size, Elf32_Addr addr) {
    addr -= 0x80000000;
    assert(addr >= 0x0 && addr < MEM_SIZE);
    for(int i = 0;i < size; i++) {
        memory[addr + i] = dat[i];
    }
}

bool loadElf(string path) {
    elfio reader;
    if ( !reader.load( path ) ) {
        cout << "Can't find or process ELF file " << path << endl;
        return false;
    }else{
        cout<<path<<endl;
        Elf_Half seg_num = reader.segments.size(); 
        for ( int i = 0; i < seg_num; ++i ) {
            const segment* pseg = reader.segments[i];
            Elf32_Addr p_addr = pseg->get_physical_address();
            if(p_addr == 0) continue;
            const char* dat = pseg->get_data();
            writeSegment(dat,pseg->get_file_size(),  p_addr);
        }
        
    }
    return true;
}

string hex(int num) {
    stringstream ss;
    ss <<"0x"<<setw(8)<<setfill('0')<<hex<<num;
    return ss.str();
}

void dump() {
    for(int i =0; i < 32; i++) {
        cout<<"x"<<i<<":\t"<<hex(regs[i])<<" ";
        if(i % 4 == 3) cout<<endl;
    }
    cout<<"PC:\t"<<hex(regs[PC])<<endl;
    cout<<dec;
}

int r32(Elf32_Addr addr) {
    addr -= 0x80000000;
    assert(addr >= 0x0 && addr < MEM_SIZE);
    int ins; 
    memcpy(&ins, memory + addr, 4);
    return ins;
}



bool step() {
    // FETCH
    int ins = r32(regs[PC]);
    regs[PC] += 4;
    char opcode = gibi(ins, 6, 0);
    
    switch(opcode) {
        case Ops::LUI:
            cout<<"LUI"<<endl;
            break;
        case Ops::LOAD:
            cout<<"LOAD"<<endl;
            break;
        case Ops::STORE:
            cout<<"STORE"<<endl;
            break;
        case Ops::AUIPC: // U-type instruction
            cout<<"AUIPC"<<endl;
            auipc(ins);
            return true;
        case Ops::JAL: // J-type instruction
            cout<<"JAL"<<endl;
            jal(ins);
            return true;
        case Ops::JALR:
            cout<<"JALR"<<endl;
            break;
        case Ops::BRANCH:
            cout<<"BRANCH"<<endl;
            break;
        case Ops::IMM: // I-type instruction
            cout<<"IMM";
            imm(ins);
            return true;
        case Ops::OP:
            cout<<"OP"<<endl; // R-type instruction
            op(ins);
            return true;
        case Ops::MISC:
            cout<<"FENCE"<<endl;
            break;
        case Ops::SYSTEM:
            cout<<"SYSTEM"<<endl;
            system_(ins);
            return true;
    }
    cout<<hex(ins)<< endl;
    dump();
    return false;
}

void test(string path) {
    assert(loadElf(path));
    regs[PC] = 0x80000000;
    while (step()) {
        continue;
    }
}


int main() {
    regs[0] = 0;
    string testPath = "riscv-tests/isa";
    vector<string> testFns = getTests(testPath);
    for(string fn : testFns) {
        test(fn);
        break;
    }

    return 0;
}