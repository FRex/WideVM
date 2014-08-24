/* 
 * File:   WideVM.hpp
 * Author: frex
 *
 * Created on August 24, 2014, 3:13 PM
 */

#ifndef VM_HPP
#define	VM_HPP

#include <vector>
#include <string>
#include <random>

namespace wvm {

class VMLocation
{
    friend class WideVM;
public:

    VMLocation() = default;

    VMLocation(const char * name) : Name(name) { }

    VMLocation(const std::string& name) : Name(name.c_str()) { }

    bool valid() const
    {
        return Name != nullptr || Location != -1;
    }
    
private:
    const char * Name = nullptr;
    int Location = -1;

};

class WideVM
{
public:
    std::vector<float> data;
    std::vector<short> program;

    std::vector<int> subprograms;

    std::vector<float> globals; //globals, read only to script -- for now



    WideVM();
    void runVmProgram(VMLocation loc, int b = -1, int e = -1);
    int particleCount() const;
    float * getParticle(int index);
    void findSubprograms();
    void addParticles(int amount, VMLocation runprogram = VMLocation());

    void setGlobal(VMLocation loc, float value);
    float getGlobal(VMLocation loc) const;
    VMLocation getGlobalLocation(const std::string& name) const;

    bool loadAsmProgram(const std::string& programcode, std::string * error = nullptr); //load entire asm program

private:
    //helpers for opcodes:
    int fetch(); //fetch a value from bytecode and increment program counter
    float read(int index); //read a particle field or constant
    float& write(int index); //write to a particle field
    void startLoop();
    bool loopParticles();

    //opcode calls:
    void opQuit();
    void opSin();
    void opCos();
    void opCopy();
    void opRand2();
    void opMath2();

    //strings:
    std::vector<std::string> m_channelnames;
    std::vector<std::string> m_globalnames;
    std::vector<std::string> m_prognames;

    //range for current program:
    int m_begin;
    int m_end;
    int m_pindex; //which particle is being done now

    int m_particlesize;
    int m_programcounter;

    //randomness
    std::mt19937 m_twister;
    std::uniform_real_distribution<float> m_normalrand = std::uniform_real_distribution<float>(0.f, 1.f);
};

} //wvm

#endif	/* VM_HPP */

