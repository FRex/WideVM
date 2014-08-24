#include "WideVM.hpp"
#include "VMOpcodes.hpp"
#include <fstream>
#include <cstring>

namespace wvm {

void WideVM::init(int size, int count, const float* initvals)
{
    particlesize = size;
    data.resize(size*count, 0.f);
    if(initvals)
    {
        for(int i = 0; i < particleCount(); ++i)
        {
            std::memcpy(getParticle(i), initvals, sizeof (float)* size);
        }
    }
}

void WideVM::runVmProgram()
{
    pc = 0;
    while(pc < program.size())
    {
        switch(program[pc])
        {
            case EVO_ACCUMULATE:
                opAccumulate();
                break;
        } //switch opcodes pc
        ++pc;
    } //while pc < opcodes size
}

int WideVM::particleCount() const
{
    return data.size() / particlesize;
}

float* WideVM::getParticle(int index)
{
    return data.data() + index * particlesize;
}

void WideVM::opAccumulate()
{
    const int accu = program[++pc];
    const int operand = program[++pc];

    for(int i = 0; i < particleCount(); ++i)
    {
        float * ptr = getParticle(i);
        ptr[accu] += ptr[operand];
    }
}

void WideVM::loadProgram(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    int opcode;
    program.clear();
    while(file >> opcode)
        program.push_back(opcode);
}

} //wvm
