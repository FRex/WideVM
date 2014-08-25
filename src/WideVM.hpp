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

inline int findStrInVector(const char * str, const std::vector<std::string>& vec)
{
    if(str)
        for(int i = 0; i < vec.size(); ++i)
            if(vec[i] == str) return i;

    return -1;
}

class VMLocation
{
    friend class WideVM;
    friend class Particle;
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

class Particle
{
    friend class WideVM;
public:

    float * getRaw()
    {
        return Ptr;
    }

    float getField(VMLocation loc)
    {
        if(loc.Location == -1)
            loc.Location = findStrInVector(loc.Name, *Channels);
        if(loc.Location == -1) return 0.f;
        return Ptr[loc.Location];
    }

    void setField(VMLocation loc, float value)
    {
        if(loc.Location == -1)
            loc.Location = findStrInVector(loc.Name, *Channels);
        if(loc.Location == -1) return;
        Ptr[loc.Location] = value;
    }

private:

    Particle(float * ptr, const std::vector<std::string>* cha) : Ptr(ptr), Channels(cha) { }

    float * Ptr;
    const std::vector<std::string> * Channels;

};

class WideVM
{
public:
    WideVM();
    
    //run program on specified range, default - entire range
    void runVmProgram(VMLocation loc, int b = -1, int e = -1);
    
    //get particle count
    int particleCount() const;
    
    //add new particles to end and optionally run a subprogram on them
    void addParticles(int amount, VMLocation runprogram = VMLocation());
    
    //get particle at index
    Particle getParticle(int index);
    
    //set global value by location
    void setGlobal(VMLocation loc, float value);
    
    //get global value by location
    float getGlobal(VMLocation loc) const;
    
    //get location of global value
    VMLocation getGlobalLocation(const std::string& name) const;

    //load entire progra(both HEADER and ASM code) from programcode STRING
    bool loadAsmProgram(std::string programcode, std::string * error = nullptr);

    //get location of particle field
    VMLocation getFieldLocation(const std::string& name) const;

private:
    void findSubprograms(); //find subprogram indices inside current program

    //helpers for opcodes:
    float * getParticleData(int index);
    int fetch(); //fetch a value from bytecode and increment program counter
    float read(int index); //read a particle field or constant(global)
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

    //program, subprograms, particle data and globals
    std::vector<float> m_particles;
    std::vector<short> m_program;
    std::vector<int> m_subprograms;
    std::vector<float> m_globals; //globals, read only to script -- for now

    //range for current program:
    int m_begin;
    int m_end;
    int m_pindex; //which particle is being done now

    int m_particlesize;
    int m_programcounter;

    //randomness source
    std::mt19937 m_twister;
};

} //wvm

#endif	/* VM_HPP */

