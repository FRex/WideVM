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


    EVO_OPCODES_COUNT //count, keep last, 'invalid'
};

}

#endif	/* VMOPCODES_HPP */

