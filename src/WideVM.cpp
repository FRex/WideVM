#include "WideVM.hpp"
#include "VMOpcodes.hpp"
#include "Assembler.hpp"
#include <cstring>
#include <cmath>
#include <ctime>

namespace wvm {

WideVM::WideVM()
{
    m_twister.seed(std::time(nullptr));
}

void WideVM::runVmProgram(VMLocation loc, int b, int e)
{
    m_begin = (b == -1)?0:b;
    m_end = (e == -1)?particleCount():e;

    if(loc.Location == -1)
        loc.Location = findStrInVector(loc.Name, m_prognames);

    if(loc.Location == -1) return;

    m_programcounter = m_subprograms[loc.Location];
    while(m_programcounter < m_program.size())
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
    return m_particles.size() / m_particlesize;
}

float* WideVM::getParticleData(int index)
{
    return m_particles.data() + index * m_particlesize;
}

void WideVM::findSubprograms()
{
    m_subprograms.clear();
    for(int i = 0; i < m_program.size(); ++i)
    {
        if(m_program[i] == EVO_SUBPROGRAM)
        {
            m_subprograms.push_back(i);
        }
        else
        {
            i += opcodeArgCount(static_cast<EVM_OPCODE>(m_program[i]));
        }
    }
}

void WideVM::addParticles(int amount, VMLocation runprogram)
{
    const int oldsize = particleCount();
    m_particles.resize(m_particles.size() + amount * m_particlesize, 0.f);
    if(runprogram.valid())
    {
        runVmProgram(runprogram, oldsize);
    }
}

Particle WideVM::getParticle(int index)
{
    return Particle(getParticleData(index), &m_channelnames);
}

void WideVM::setGlobal(VMLocation loc, float value)
{
    if(loc.Location == -1)
        loc.Location = findStrInVector(loc.Name, m_globalnames);

    if(loc.Location == -1) return;
    m_globals[loc.Location] = value;
}

float WideVM::getGlobal(VMLocation loc) const
{
    if(loc.Location == -1)
        loc.Location = findStrInVector(loc.Name, m_globalnames);

    if(loc.Location == -1) return 0.f;
    return m_globals[loc.Location];
}

VMLocation WideVM::getGlobalLocation(const std::string& name) const
{
    VMLocation ret;
    ret.Location = findStrInVector(name.c_str(), m_globalnames);
    return ret;
}

VMLocation WideVM::getFieldLocation(const std::string& name) const
{
    VMLocation ret;
    ret.Location = findStrInVector(name.c_str(), m_channelnames);
    return ret;
}

bool WideVM::loadAsmProgram(std::string programcode, std::string * error)
{
    const std::size_t atmark = programcode.find_first_of('@');
    if(atmark == std::string::npos)
    {
        if(error)
            (*error) = "no @ char (separator of header and code) found";

        return false;
    }

    stripCppComments(programcode);

    if(!bakeHeader(programcode.substr(0, atmark + 1u), m_globalnames, m_globals, m_channelnames, m_prognames, error))
    {
        return false;
    }

    m_particlesize = m_channelnames.size();

    if(!assemble(programcode.substr(atmark + 1u), m_program, error))
    {
        return false;
    }

    findSubprograms(); //check if we have any sub progs?

    if(m_subprograms.empty())
    {
        if(error)
            (*error) = "there are no subprograms in this program";

        return false;
    }

    return true;
}

//helpers for opcodes:

int WideVM::fetch()
{
    return m_program[m_programcounter++];
}

float WideVM::read(int index)
{
    if(index < 0)
    {
        index = -index - 1;
        return m_globals[index];
    }
    return getParticleData(m_pindex)[index];
}

float& WideVM::write(int index)
{
    return getParticleData(m_pindex)[index];
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
    m_programcounter = m_program.size(); //trick program into thinking codes ran out
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
