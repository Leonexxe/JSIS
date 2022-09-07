/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/genAsm.cpp                                                             #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, September 2nd 2022, 6:38:04 pm                                                       #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
#pragma once
#include "master.h"

void genFunc(string name,list<var>& args)
{
    assembly+=getFunctionLabel(name,args)+":\n";
}

void genReturnLiteral(string value)
{
    assembly+="emov rv "+value+"\n";
    assembly+="return\n";
}

void genReturn(u64 value)
{
    assembly+="ewrite rv "+std::to_string(value)+"\n";
    assembly+="return\n";
}

void genValueAssign(u64 obj,u64 value)
{
    assembly+="write "+std::to_string(obj)+" "+std::to_string(value)+"\n";
}

void genWriteLiteral(string reg,string value)
{
    assembly+="mov "+reg+" "+value+"\n";
}

void genWriteLiteral(u64 obj,string value)
{
    assembly+="mov "+std::to_string(obj)+" "+value+"\n";
}

void genArrayWriteLiteral(u64 aObj,u64 aOffset,string literal)
{
    assembly+="alwrite "+std::to_string(aObj)+" "+std::to_string(aOffset)+" "+literal+"\n";
}
void genArrayWrite(u64 aObj,u64 aOffset,u64 vObj)
{
    assembly+="awrite "+std::to_string(aObj)+" "+std::to_string(aOffset)+" "+std::to_string(vObj)+"\n";
}

void genMalloc(u64 obj,u64 bytes)
{
    assembly+="malloc "+std::to_string(obj)+" "+std::to_string(bytes)+"\n";
}

void genFree(u64 obj)
{
    assembly+="free "+std::to_string(obj)+"\n";
}


void genEngineCall(u64 function,list<var> args)
{
}

void genFunctionCall(u64 returnObj,u64 targetObj,string func,list<var> args)
{
    string fLabel = getFunctionLabel(func,args);
    u64 arg = 0;
    assembly+="set AAS "+std::to_string(args.size())+"\n";
    assembly+="pusha\n";
    for(var& v : args)
        assembly+="aewrite args "+std::to_string(arg++)+" "+std::to_string(v.number)+"\n";
    assembly+="push this "+std::to_string(targetObj)+"\n";
    assembly+="push rv "+std::to_string(returnObj)+"\n";
    assembly+="push ret $+1\n";
    assembly+="jmp "+fLabel+"\n";
    assembly+="pop ret\n";
    assembly+="pop this\n";
    assembly+="pop rv\n";
    assembly+="popa\n";
}

void genMakeVar()
{
}
