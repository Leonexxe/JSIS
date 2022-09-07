/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/assembler.cpp                                                          #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Sunday, September 4th 2022, 9:28:10 am                                                       #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
#pragma once
#include "master.h"

list<u64> instructions;

#define ASS_MOV     0x00 // mov [obj] [literal]         // writes a literal value to an address
#define ASS_RET     0x01 // ret                         // jumps to the top of the ret stack
#define ASS_WRITE   0x02 // write [obj] [obj]           // writes the value of the second to the first object
#define ASS_PUSHA   0x03 // pusha                       // pushes an args object onto the args stack
#define ASS_POPA    0x04 // popa                        // pops the args stack
#define ASS_PUSH    0x05 // push [stack] [obj]          // pushes the objects value onto the stack
#define ASS_POP     0x06 // pop  [stack]                // pops the stack
#define ASS_JMP     0x07 // jmp  [symbol]               // jumps to a symbol
                         // ljmp [literal]              // jumps to a literal address
#define ASS_VJMP    0x08 // vjmp [obj]                  // jumps to the address given by the objects value
#define ASS_SET     0x09 // set  [settable] [literal]   // sets engine values such as the EOO to a literal value
#define ASS_EMOV    0x0A // emov [EO] [literal]         // write the value of an engine defined object to a literal
#define ASS_EWRITE  0x0B // ewrite [EO] [obj]           // write the value of an engine defined object to the value of an object
#define ASS_CALL    0x0C // call [EFN]                  // call a function defined and implemented by the engine by its number
#define ASS_GET     0x0D // get [obj] [EON]             // get the value of an engine defined object and write it to an object
#define ASS_AWRITE  0x0E // awrite [obj] [obj] [obj]    // write the value of the third object to obj1[obj2]
#define ASS_ALWRITE 0x0F // awrite [obj] [obj] [literal]// write a literal value to obj1[obj2]
#define ASS_HLT     0x10 // hlt                         // halt execution
#define ASS_PAUSE   0x11 // pause                       // pause execution until hit up by another thread
#define ASS_EXEC    0x12 // exec [obj] [obj]            // kick of another thread at the address provided by the value of an object thread id will then be written to the second object
#define ASS_RESUME  0x13 // resume [obj]                // resume execution on another thread
#define ASS_MALLOC  0x14 // malloc [obj] [literal]      // tell the engine allocate a number of bytes for the object
#define ASS_FREE    0x15 // free [obj]                  // tell the engine to deallocate (free) the memory that was allocated for the object
#define ASS_INT     0x16 // int [literal]               // works like assembly interrupts, triggers engine defined behavior
#define ASS_AEWRITE 0x17
#define ASS_AELWRITE 0x18//

u64 ASS_EOO = 1000000;

struct assSymbol
{
    assSymbol(string name, u64 addr)
        :name(name),addr(addr){}
    string name;
    u64 addr;
};
list<assSymbol> assSymbols;

void assError(string ass)
{
    std::cout << "[ERROR] " << ass << std::endl;
    exit(1);
}

u64 sti(string s)
{
    if(s[0] == '$')
        return instructions.size()+(std::stoll(s.substr(1,s.size()-1)));
    return std::stoull(s);
}

u64 assGetArray(string name)
{
    if(name == "args")   return 0;
    assError("Array not found \""+name+"\" !");
}

u64 assGetSettable(string name)
{
    if(name == "EOO")   return 0;
    if(name == "AAS")   return 0;
    assError("settable not found \""+name+"\" !");
}

u64 assGetEVal(string name)
{
    //specific registers
    if(name == "rv")    return 0;
    //general purpose registers
    if(name == "rax")  return 0x10;
    if(name == "rbx")  return 0x11;
    if(name == "rcx")  return 0x12;
    if(name == "rdx")  return 0x13;
    if(name == "rex")  return 0x14;
    if(name == "rfx")  return 0x15;
    if(name == "rgx")  return 0x16;
    if(name == "rhx")  return 0x17;
    if(name == "rix")  return 0x18;
    if(name == "rjx")  return 0x19;
    if(name == "rkx")  return 0x1A;
    if(name == "rlx")  return 0x1B;
    if(name == "rmx")  return 0x1C;
    if(name == "rox")  return 0x1D;
    if(name == "rpx")  return 0x1E;
    if(name == "rqx")  return 0x1F;
    assError("register not found \""+name+"\" !");
}

u64 assGetStack(string name)
{
    //specific stacks
    if(name == "ret")   return 0x0;
    if(name == "rv")    return 0x1;
    if(name == "this")  return 0x2;
    //general purpose stacks
    if(name == "sax")  return 0x10;
    if(name == "sbx")  return 0x11;
    if(name == "scx")  return 0x12;
    if(name == "sdx")  return 0x13;
    if(name == "sex")  return 0x14;//i couldn't not
    if(name == "sfx")  return 0x15;
    if(name == "sgx")  return 0x16;
    if(name == "shx")  return 0x17;
    assError("stack not found \""+name+"\" !");
}

u64 assGetSymbolAddr(string name)
{
    for(assSymbol I : assSymbols)
        if(I.name == name)
            return I.addr;
    assError("symbol not found \""+name+"\" !");
}

list<string> fLines;
u64 assSSICounter = 0;
void assSymbolScan(string line)
{
    if(line.back() == ':')
    {
        assSymbols.push_back(assSymbol(line.substr(0,line.size()-1),assSSICounter+1));
        std::cout << "defined symbol " << assSymbols.back().name << " with addr " << assSSICounter << std::endl;
        return;
    }

    for(char I : line)
        if(I == ' ')
            assSSICounter++;
    if(!empty(line))
        fLines.push_back(line);
}
 
u64 OIS = 0;
void assembleLine(string& line)
{
    if(line[0] == '/' && line[1] == '/')
    {
        if(line.size()>sizeof("//@file "))
        {
            if(line.substr(0,sizeof("//@file ")-1) == "//@file ")
            {
                //save old file
                for(codeFile& cf : CFlist)
                {
                    if(cf.name == currentFile)
                    {
                        cf.line = lineCounter;
                        break;
                    }
                }

                //set new file
                currentFile = strSplit(line," ",0,0)[1];
                for(codeFile& cf : CFlist)
                {
                    if(cf.name == currentFile)
                    {
                        lineCounter = cf.line;
                        goto _1_skip;
                    }
                }
                lineCounter = 0;
                CFlist.push_back(codeFile(currentFile,lineCounter));
                _1_skip: ;
                if(f_Verbose)   
                    std::cout << "compiling file: " << currentFile << std::endl;
            }
        }
        return;
    }
    if(f_Debug)
        std::cout << "line: " << line << std::endl;

    list<string> t = strSplit(line," ",0,0);
    if(t[0] == "mov")
    {
        if(t.size() < 3)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_MOV);
        instructions.push_back(sti(t[1]));
        instructions.push_back(sti(t[2]));
        goto _assLine_end;
    }
    if(t[0] == "write")
    {
        if(t.size() < 3)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_WRITE);
        instructions.push_back(sti(t[1]));
        instructions.push_back(sti(t[2]));
        goto _assLine_end;
    }
    if(t[0] == "pusha")
    {
        if(t.size() < 1)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_PUSHA);
        goto _assLine_end;
    }
    if(t[0] == "popa")
    {
        if(t.size() < 1)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_POPA);
        goto _assLine_end;
    }
    if(t[0] == "push")
    {
        if(t.size() < 3)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_PUSH);
        instructions.push_back(assGetStack(t[1]));
        instructions.push_back(sti(t[2]));
        goto _assLine_end;
    }
    if(t[0] == "pop")
    {
        if(t.size() < 2)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_POP);
        instructions.push_back(assGetStack(t[1]));
        goto _assLine_end;
    }
    if(t[0] == "jmp")
    {
        if(t.size() < 2)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_JMP);
        instructions.push_back(assGetSymbolAddr(t[1]));
        goto _assLine_end;
    }
    if(t[0] == "ljmp")
    {
        if(t.size() < 2)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_JMP);
        instructions.push_back(sti(t[1]));
        goto _assLine_end;
    }
    if(t[0] == "vjmp")
    {
        if(t.size() < 2)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_VJMP);
        instructions.push_back(sti(t[1]));
        goto _assLine_end;
    }
    if(t[0] == "return")
    {
        if(t.size() < 1)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_RET);
        goto _assLine_end;
    }
    if(t[0] == "set")
    {
        if(t.size() < 3)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_SET);
        instructions.push_back(assGetSettable(t[1]));
        instructions.push_back(sti(t[2]));
        if(t[1] == "EOO")
        {
            ASS_EOO = sti(t[2]);
            std::cout << "[INFO] EOO set to " << t[2] << std::endl;
        }
        goto _assLine_end;
    }
    if(t[0] == "emov")
    {
        if(t.size() < 3)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_EMOV);
        instructions.push_back(assGetEVal(t[1]));
        instructions.push_back(sti(t[2]));
        goto _assLine_end;
    }
    if(t[0] == "ewrite")
    {
        if(t.size() < 3)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_EWRITE);
        instructions.push_back(assGetEVal(t[1]));
        instructions.push_back(sti(t[2]));
        goto _assLine_end;
    }
    if(t[0] == "call")
    {
        if(t.size() < 2)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_CALL);
        instructions.push_back(sti(t[1]));
        goto _assLine_end;
    }
    if(t[0] == "awrite")
    {
        if(t.size() < 4)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_AWRITE);
        instructions.push_back(sti(t[1]));
        instructions.push_back(sti(t[2]));
        instructions.push_back(sti(t[3]));
        goto _assLine_end;
    }
    if(t[0] == "alwrite")
    {
        if(t.size() < 4)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_ALWRITE);
        instructions.push_back(sti(t[1]));
        instructions.push_back(sti(t[2]));
        instructions.push_back(sti(t[3]));
        goto _assLine_end;
    }
    if(t[0] == "aewrite")
    {
        if(t.size() < 4)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_AEWRITE);
        instructions.push_back(assGetArray(t[1]));
        instructions.push_back(sti(t[2]));
        instructions.push_back(sti(t[3]));
        goto _assLine_end;
    }
    if(t[0] == "aelwrite")
    {
        if(t.size() < 4)
            assError("too little arguments for "+t[0]+" instruction!");
        instructions.push_back(ASS_AELWRITE);
        instructions.push_back(assGetArray(t[1]));
        instructions.push_back(sti(t[2]));
        instructions.push_back(sti(t[3]));
        goto _assLine_end;
    }
    assError("instruction not found \""+t[0]+"\" at "+getCodePos());
    _assLine_end: ;
    std::cout << "assembly: ";
    for(OIS=OIS;OIS<instructions.size();OIS++)
        std::cout << instructions[OIS] << " ";
    std::cout << std::endl;
}

void assemble()
{
    list<string> lines = strSplit(assembly,"\n",0,0);
    stripList(lines);
    for(string I : lines)
        assSymbolScan(I);
    for(string& I : fLines)
        assembleLine(I);

    u64* instr = new u64[instructions.size()];
    for(u64 I = 0;I<instructions.size();I++)
        instr[I] = instructions[I];
    u8* WI = (u8*)instr;

    // Open a file for writing. 
    // (This will replace any existing file. Use "w+" for appending)
    FILE* file = fopen(outputFile.c_str(), "w");
    
    int results = fputs((const char*)WI, file);
    if (results == EOF) {
        std::cout << "failed to write to " << outputFile;
    }
    fclose(file);
}

void writeAsm()
{
    // Open a file for writing. 
    // (This will replace any existing file. Use "w+" for appending)
    FILE* file = fopen(outputFile.c_str(), "w");
    
    int results = fputs(assembly.c_str(), file);
    if (results == EOF) {
        std::cout << "failed to write to " << outputFile;
    }
    fclose(file);
}