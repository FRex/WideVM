/* 
 * File:   VMOpcodes.hpp
 * Author: frex
 *
 * Created on August 24, 2014, 3:12 PM
 */

#ifndef VMOPCODES_HPP
#define	VMOPCODES_HPP

namespace wvm {

enum EVM_OPCODE
{
    EVO_QUIT = 0, //0 args -- quit the current program
    EVO_ACCUMULATE = 1, //2 args -- add 2nd arg to 1st
    EVO_SUBPROGRAM = 2, //0 args -- beginning of a sub program

    EVO_OPCODES_COUNT //keep last, 'invalid' opcode, not EVO_COUNT in case there is a real count opcode later
};

int opcodeArgCount(EVM_OPCODE code);

const char * opcodeToStr(EVM_OPCODE code);
EVM_OPCODE strToOpcode(const char * str, int size = -1);

}

#endif	/* VMOPCODES_HPP */

