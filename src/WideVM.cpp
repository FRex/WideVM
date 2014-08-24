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
        const int opcode = fetch();
        switch(opcode)
        {
            case EVO_QUIT:
                opQuit();
                break;
            case EVO_SIN:
                opSin();
                break;
            case EVO_COS:
                opCos();
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

int WideVM::fetch()
{
    return program[pc++];
}

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

void WideVM::opSin()
{
    const int result = fetch();
    const int arg = fetch();

    while(loopParticles())
    {
        write(result) = std::sin(read(arg));
    }
}

void WideVM::opCos()
{
    const int result = fetch();
    const int arg = fetch();

    while(loopParticles())
    {
        write(result) = std::cos(read(arg));
    }
}

void WideVM::opCopy()
{
    const int to = fetch();
    const int from = fetch();

    while(loopParticles())
    {
        write(to) = read(from);
    }
}

void WideVM::opRand2()
{
    const int to = fetch();
    const int min = fetch();
    const int max = fetch();

    while(loopParticles())
    {
        std::uniform_real_distribution<float> distri(read(min), read(max));
        write(to) = distri(m_twister);
    }
}

void WideVM::opMath2()
{
    const int op = fetch();
    const int to = fetch();
    const int arg1 = fetch();
    const int arg2 = fetch();
    while(loopParticles())
    {
        switch(op)
        {
            case 1:
                write(to) = read(arg1) + read(arg2);
                break;
            case 2:
                write(to) = read(arg1) - read(arg2);
                break;
            case 3:
                write(to) = read(arg1) * read(arg2);
                break;
        }
    } //loopParticles
}

} //wvm
