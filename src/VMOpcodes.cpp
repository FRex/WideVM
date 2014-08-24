#include "VMOpcodes.hpp"
#include <cstring>

namespace wvm {

namespace {

class OpcodeInfo
{
public:
    EVM_OPCODE Opcode; //opcode itself, in case they aren't consecutive later
    int Arguments; //number of arguments -- fixed for now
    const char * String; //string representation for asm
};

const OpcodeInfo infos[] = {
    {EVO_QUIT, 0, "quit"},
    {EVO_SUBPROGRAM, 0, "subprog"},
    {EVO_SIN, 2, "sin"},
    {EVO_COS, 2, "cos"},
    {EVO_COPY, 2, "copy"},
    {EVO_RAND2, 3, "rand2"},
    {EVO_MATH2, 4, "math2"},


    {EVO_OPCODES_COUNT, 0, ""} //keep last!
};

//+1 is for count opcode, so it doesn't need to be special in argcount and tostr
static_assert(sizeof (infos) / sizeof (infos[0]) == EVO_OPCODES_COUNT + 1, "missing some opcodes info");

} //unnamed

int opcodeArgCount(EVM_OPCODE code)
{
    return infos[code].Arguments;
}

const char * opcodeToStr(EVM_OPCODE code)
{
    return infos[code].String;
}

EVM_OPCODE strToOpcode(const char * str, int size)
{
    if(size == -1)
        size = std::strlen(str);

    for(int i = 0; i < EVO_OPCODES_COUNT; ++i)
        if(0 == std::strncmp(str, infos[i].String, size)) return infos[i].Opcode;

    return EVO_OPCODES_COUNT;
}

} //wvm
