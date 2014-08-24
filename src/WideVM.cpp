#include "WideVM.hpp"
#include "VMOpcodes.hpp"
#include "Assembler.hpp"
#include <fstream>
#include <cstring>
#include <cmath>

namespace wvm {

static int findStrInVector(const char * str, const std::vector<std::string>& vec)
{
    if(str)
        for(int i = 0; i < vec.size(); ++i)
            if(vec[i] == str) return i;

    return -1;
}

WideVM::WideVM()
{
    m_twister.seed(std::rand());
}

void WideVM::runVmProgram(VMLocation loc, int b, int e)
{
    m_begin = (b == -1)?0:b;
    m_end = (e == -1)?particleCount():e;

    if(loc.Location == -1)
        loc.Location = findStrInVector(loc.Name, m_prognames);

    m_programcounter = subprograms[loc.Location];
    while(m_programcounter < program.size())
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
    } //while pc < opcodes size
}

int WideVM::particleCount() const
{
    return data.size() / m_particlesize;
}

float* WideVM::getParticle(int index)
{
    return data.data() + index * m_particlesize;
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

void WideVM::addParticles(int amount, VMLocation runprogram)
{
    const int oldsize = particleCount();
    data.resize(data.size() + amount * m_particlesize, 0.f);
    if(runprogram.valid())
    {
        runVmProgram(runprogram, oldsize);
    }
}

void WideVM::setGlobal(VMLocation loc, float value)
{
    if(loc.Location == -1)
        loc.Location = findStrInVector(loc.Name, m_globalnames);

    globals[loc.Location] = value;
}

float WideVM::getGlobal(VMLocation loc) const
{
    if(loc.Location == -1)
        loc.Location = findStrInVector(loc.Name, m_globalnames);

    return globals[loc.Location];
}

VMLocation WideVM::getGlobalLocation(const std::string& name) const
{
    VMLocation ret;
    ret.Location = findStrInVector(name.c_str(), m_globalnames);
    return ret;
}

bool WideVM::loadAsmProgram(const std::string& programcode, std::string * error)
{
    const std::size_t atmark = programcode.find_first_of('@');
    if(atmark == std::string::npos)
    {
        if(error)
            (*error) = "no @ char (separator of header and code) found";

        return false;
    }


    if(!bakeHeader(programcode.substr(0, atmark + 1u), m_globalnames, globals, m_channelnames, m_prognames, error))
    {
        return false;
    }

    m_particlesize = m_channelnames.size();

    if(!assemble(programcode.substr(atmark + 1u), program, error))
    {
        return false;
    }

    findSubprograms(); //check if we have any sub progs?

    return true;
}

//helpers for opcodes:

int WideVM::fetch()
{
    return program[m_programcounter++];
}

float WideVM::read(int index)
{
    if(index < 0)
    {
        index = -index - 1;
        return globals[index];
    }
    return getParticle(m_pindex)[index];
}

float& WideVM::write(int index)
{
    return getParticle(m_pindex)[index];
}

void WideVM::startLoop()
{
    m_pindex = m_begin - 1;
}

bool WideVM::loopParticles()
{
    ++m_pindex;
    if(m_pindex >= m_end) return false;
    return true;
}

//opcode calls:

void WideVM::opQuit()
{
    m_programcounter = program.size(); //trick program into thinking codes ran out
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
            case 4:
                write(to) = read(arg1) / read(arg2);
                break;
        }
    } //loopParticles
}

} //wvm
