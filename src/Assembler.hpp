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

void stripCppComments(std::string& str);

bool assemble(std::string code, std::vector<short>& outopcodes, std::string * error = nullptr);

#define vecs std::vector<std::string>&
bool bakeHeader(std::string code, vecs globals, std::vector<float>& values, vecs channels, vecs pnames, std::string * error = nullptr);
#undef vecs

}

#endif	/* ASSEMBLER_HPP */

