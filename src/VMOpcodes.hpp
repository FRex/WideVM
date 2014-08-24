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
    EVO_DEACCUMULATE = 3, //2 args -- subtract 2nd arg from 1st

    EVO_SIN = 4, //2 args -- put into 1st the value of sin(2nd)
    EVO_COS = 5, //2 args -- put into 1st the value of cos(2nd)
    EVO_RAND = 6, //1 arg -- put into it the rand 0..1 value
    EVO_COPY = 7, //2 args -- copy from 2nd into 1st
    EVO_RAND2 = 8, //3 args -- put into 1st the value of random(2nd,3rd)
    EVO_MATH2 = 9, //4 args -- first is op, 2nd is place to put result, 3rd and 4th are operands

    EVO_OPCODES_COUNT //keep last, 'invalid' opcode, not EVO_COUNT in case there is a real count opcode later
};

int opcodeArgCount(EVM_OPCODE code);

const char * opcodeToStr(EVM_OPCODE code);
EVM_OPCODE strToOpcode(const char * str, int size = -1);

}

#endif	/* VMOPCODES_HPP */

