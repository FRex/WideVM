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

class WideVM
{
public:
    std::vector<float> data;
    std::vector<short> program;

    std::vector<int> subprograms;

    std::vector<float> globals; //globals, read only to script -- for now

    int particlesize;
    int pc;

    //range for current program:
    int begin;
    int end;

    //which particle is being done now
    int pindex;

    WideVM();

    void init(int size, int count, const float * initvals = nullptr);
    void runVmProgram(int subprog, int b = -1, int e = -1);
    int particleCount() const;
    float * getParticle(int index);
    void findSubprograms();

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


    std::mt19937 m_twister;
    std::uniform_real_distribution<float> m_normalrand = std::uniform_real_distribution<float>(0.f, 1.f);
};

} //wvm

#endif	/* VM_HPP */

