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

bool assemble(const char * code, std::vector<short>& outopcodes, std::string * error = nullptr);

}

#endif	/* ASSEMBLER_HPP */

