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
    std::vector<char> program;

    int particlesize;
    int pc;

    void init(int size, int count, const float * initvals = nullptr);
    void runVmProgram();
    int particleCount() const;
    float * getParticle(int index);
    void loadProgram(const std::string& filename);

private:
    //opcode calls:
    void opQuit();
    void opAccumulate();


};

} //wvm

#endif	/* VM_HPP */

