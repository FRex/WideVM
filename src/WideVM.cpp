#include "WideVM.hpp"
#include "VMOpcodes.hpp"
#include <fstream>
#include <cstring>
#include <cmath>

namespace wvm {

WideVM::WideVM()
{
    m_twister.seed(std::rand());
}

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
        startLoop(); //in case an opcode uses while loopParticle init counter for it
        switch(program[pc])
        {
            case EVO_QUIT:
                opQuit();
                break;
            case EVO_ACCUMULATE:
                opAccumulate();
                break;
            case EVO_DEACCUMULATE:
                opDeaccumulate();
                break;
            case EVO_SIN:
                opSin();
                break;
            case EVO_COS:
                opCos();
                break;
            case EVO_RAND:
                opRand();
                break;
            case EVO_COPY:
                opCopy();
                break;
            case EVO_RAND2:
                opRand2();
                break;
            case EVO_MATH2:
                opMath2();
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

//helpers for opcodes:

float WideVM::read(int index)
{
    if(index < 0)
    {
        index = -index - 1;
        return globals[index];
    }
    return getParticle(pindex)[index];
}

float& WideVM::write(int index)
{
    return getParticle(pindex)[index];
}

void WideVM::startLoop()
{
    pindex = begin - 1;
}

bool WideVM::loopParticles()
{
    ++pindex;
    if(pindex >= end) return false;
    return true;
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

void WideVM::opDeaccumulate()
{
    const int accu = program[++pc];
    const int operand = program[++pc];

    for(int i = begin; i < end; ++i)
    {
        float * ptr = getParticle(i);
        ptr[accu] -= ptr[operand];
    }
}

void WideVM::opSin()
{
    const int result = program[++pc];
    const int arg = program[++pc];

    for(int i = begin; i < end; ++i)
    {
        float * ptr = getParticle(i);
        ptr[result] = std::sin(ptr[arg]);
    }
}

void WideVM::opCos()
{
    const int result = program[++pc];
    const int arg = program[++pc];

    for(int i = begin; i < end; ++i)
    {
        float * ptr = getParticle(i);
        ptr[result] = std::cos(ptr[arg]);
    }
}

void WideVM::opRand()
{
    const int result = program[++pc];

    for(int i = begin; i < end; ++i)
    {
        float * ptr = getParticle(i);
        ptr[result] = m_normalrand(m_twister);
    }
}

void WideVM::opCopy()
{
    const int to = program[++pc];
    const int from = program[++pc];

    while(loopParticles())
    {
        write(to) = read(from);
    }
}

void WideVM::opRand2()
{
    const int to = program[++pc];
    const int min = program[++pc];
    const int max = program[++pc];

    while(loopParticles())
    {
        std::uniform_real_distribution<float> distri(read(min), read(max));
        write(to) = distri(m_twister);
    }
}

void WideVM::opMath2()
{
    const int op = program[++pc];
    const int to = program[++pc];
    const int arg1 = program[++pc];
    const int arg2 = program[++pc];
    while(loopParticles())
    {
        switch(op)
        {
            case 1:
                write(to) = read(arg1) - read(arg2);
                break;
            case 2:
                write(to) = read(arg1) + read(arg2);
                break;
            case 3:
                write(to) = read(arg1) * read(arg2);
                break;
        }
    } //loopParticles
}

} //wvm
