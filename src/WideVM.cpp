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

void WideVM::runVmProgram(int subprog, int b, int e)
{
    begin = (b == -1)?0:b;
    end = (e == -1)?particleCount():e;

    pc = subprograms[subprog];
    while(pc < program.size())
    {
        switch(program[pc])
        {
            case EVO_QUIT:
                opQuit();
                break;
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

void WideVM::findSubprograms()
{
    subprograms.clear();
    for(int i = 0; i < program.size(); ++i)
    {
        if(program[i] == EVO_SUBPROGRAM)
        {
            subprograms.push_back(i);
        }
        else
        {
            i += opcodeArgCount(static_cast<EVM_OPCODE>(program[i]));
        }
    }
}

//opcode calls:

void WideVM::opQuit()
{
    pc = program.size(); //trick program into thinking codes ran out
}

void WideVM::opAccumulate()
{
    const int accu = program[++pc];
    const int operand = program[++pc];

    for(int i = begin; i < end; ++i)
    {
        float * ptr = getParticle(i);
        ptr[accu] += ptr[operand];
    }
}

} //wvm
