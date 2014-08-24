#include "Assembler.hpp"
#include "VMOpcodes.hpp"

#include <sstream>
#include <cstdio>

namespace wvm {

static void stripAsmComments(std::string& str)
{
    bool erasing = false;
    str += ' ';
    for(std::size_t i = 0u; i < str.size(); ++i)
    {
        if(str[i] == '\n') erasing = false;
        if(str[i] == '/' && str[i + 1u] == '/') erasing = true;
        if(erasing) str[i] = ' ';
    }
}

bool assemble(std::string code, std::vector<short>& outopcodes, std::string * error)
{
    stripAsmComments(code);
    std::istringstream ss(code);
    std::string asmcommand;
    outopcodes.clear();

    while(ss >> asmcommand)
    {
        const EVM_OPCODE opcode = strToOpcode(asmcommand.c_str(), asmcommand.size());

        if(opcode == EVO_OPCODES_COUNT) //no such code
        {
            if(error)
            {
                (*error) = "'" + asmcommand + "' is not a valid command";
            }
            return false;
        } //if opcode == EVO_OPCODES_COUNT

        outopcodes.push_back(opcode);
        for(int i = 0; i < opcodeArgCount(opcode); ++i)
        {
            int arg;
            if(ss >> arg)
            {
                outopcodes.push_back(arg);
            }
            else //cant read next number
            {
                if(error)
                {
                    char buff[256];
                    std::sprintf(buff, "' needs %d args, found only %d\n", opcodeArgCount(opcode), i);
                    (*error) = "'" + asmcommand + buff;
                }
                return false;
            }
        }//for i = 0, opcodeArgCount opcode, 1
    } //while ss >> asmcommand
    return true;
}

}
