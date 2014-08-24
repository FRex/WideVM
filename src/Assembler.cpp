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

#define vecs std::vector<std::string>&

bool bakeHeader(std::string code, vecs globals, std::vector<float>& values, vecs channels, vecs pnames, std::string * error)
#undef vecs
{
    //stripAsmComments(code);
    //add error handling here?
    globals.clear();
    values.clear();
    channels.clear();
    pnames.clear();
    std::istringstream ss(code);
    std::string str;
    int count;

    ss >> count>>str; //count and GLOBALS
    for(int i = 0; i < count; ++i)
    {
        ss>>str;
        globals.push_back(str);
        float val;
        ss>>val;
        values.push_back(val);
    }

    ss >> count>>str; //count and CHANNELS
    for(int i = 0; i < count; ++i)
    {
        ss>>str;
        channels.push_back(str);
    }

    ss >> count >> str; //count and SUBS
    for(int i = 0; i < count; ++i)
    {
        ss>>str;
        pnames.push_back(str);
    }

    ss>>str; //@
    return true;
}

}
