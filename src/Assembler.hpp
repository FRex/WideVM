/* 
 * File:   Assembler.hpp
 * Author: frex
 *
 * Created on August 24, 2014, 3:41 PM
 */

#ifndef ASSEMBLER_HPP
#define	ASSEMBLER_HPP

#include <vector>
#include <string>

namespace wvm {

//strip c++ style // comments, any only them, from code
void stripCppComments(std::string& str);

//assemble the asm part of program into opcodes
bool assemble(std::string code, std::vector<short>& outopcodes, std::string * error = nullptr);

#define vecs std::vector<std::string>&
//get info about globals, names and so on from header part of program
bool bakeHeader(std::string code, vecs globals, std::vector<float>& values, vecs channels, vecs pnames, std::string * error = nullptr);
#undef vecs

}

#endif	/* ASSEMBLER_HPP */

