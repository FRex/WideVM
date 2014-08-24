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

namespace wvm {

class WideVM
{
public:
    std::vector<float> data;
    std::vector<short> program;

    std::vector<int> subprograms;

    int particlesize;
    int pc;

    //range for current program:
    int begin;
    int end;

    void init(int size, int count, const float * initvals = nullptr);
    void runVmProgram(int subprog, int b = -1, int e = -1);
    int particleCount() const;
    float * getParticle(int index);
    void findSubprograms();

private:
    //opcode calls:
    void opQuit();
    void opAccumulate();
    void opDeaccumulate();

};

} //wvm

#endif	/* VM_HPP */

