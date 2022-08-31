/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/compile.cpp                                                            #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, August 26th 2022, 1:57:53 am                                                         #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
#pragma once
#include "master.h"

#define EOO 1000000

enum objType
{
    _type,_variable,_function
};

struct type;
struct var;
struct function;
struct obj
{
    string name;
    u64 number;
    u64 subNumber;
    objType ObjType;
    u64 T;

    obj(){}
    obj(string name,u64 number,u64 subNumber,u64 T,objType ObjType)
        :name(name),number(number),subNumber(subNumber),T(T),ObjType(ObjType){}
};

struct function : public obj
{
    using obj::obj;
    list<var> args;
};

struct var : public obj
{
    using obj::obj;
};

struct type : public obj
{
    using obj::obj;
    list<u64> memberFunctions;
    list<u64> memberVariables;
};

struct objD
{
    objType ot;
    u64 subNumber;
    u64 objNumber;

    objD(objType ot,u64 subNumber,u64 objNumber)
        :ot(ot),subNumber(subNumber),objNumber(objNumber){}
};

list<function>  functions;
list<type>      types;
list<var>       vars;
list<objD>      objects;

u64 objNumber = EOO;//first n objects are defined and implemented by the engine

struct scope
{
    list<u64> sf;
    list<u64> st;
    list<u64> sv;
    scope* parent = nullptr;
};

list<scope> scopeStack;

u64 getVar(string name)
{
    for(var& I : vars)
        if(I.name == name)
            return I.number;
    return 0;
}

u64 getFunc(string name,list<var>& args)
{
    for(function& I : functions){if(I.name == name)
    {
        if(args.size() == 0)
            continue;

        if(I.args.size() != args.size())
            continue;

        list<var>& fArgs = I.args;
        var* ea;
        var* na;
        bool argsEqual = 1;
        for(int I = 0;I<args.size();I++)
        {
            ea = &fArgs[I];na = &args[I];
            if(ea->T != na->T)
            {
                argsEqual = 0;
                break;
            }
        }
        if(argsEqual)
            return I.number;
    }}
    return 0;
}

bool objExists(string name,list<var>& args)
{
    for(var& I : vars)
        if(I.name == name)
            return 1;

    for(function& I : functions){if(I.name == name)
    {
        if(args.size() == 0)
            continue;

        if(I.args.size() != args.size())
            continue;

        list<var>& fArgs = I.args;
        var* ea;
        var* na;
        bool argsEqual = 1;
        for(int I = 0;I<args.size();I++)
        {
            ea = &fArgs[I];na = &args[I];
            if(ea->T != na->T)
            {
                argsEqual = 0;
                break;
            }
        }
        if(argsEqual)
            return 1;
    }}
    return 0;
}

u64 getType(string name)
{
    for(int I = 0;I<types.size();I++)
        if(types[I].name == name)
            return types[I].number;
    std::cout << "[ERROR] type not found: " << name << std::endl;
    exit(1);
}

list<var> noArgs;
list<u64> instructions;
u64 cLevel = 0;

void compileLine_topLevel(string& code)
{
    list<string> tokens = strSplit(code," ",0,0);
    stripList(tokens);
    //? 
    //? class declarations
    //? 
    if(tokens[0] == "class")
    {
        //type
        types.push_back(type(
                            tokens[1],
                            objNumber++,
                            (u64)types.size(),
                            0,
                            _type
                            ));
        std::cout << "declared type " << tokens[1] << std::endl;
        return;
    }
    u64 T = getType(tokens[0]);
    if(strContains('(',tokens[1]))
    {
        //function declaration
        string decl;
        for(int I = 1;I<tokens.size();I++)
            decl+=tokens[I];  
        
        list<string> st = strSplit(tokens[1],"(",0,0);
        string fName = st[0];
        function f(fName,objNumber++,functions.size(),T,_function);
        if(st[1][0] != ')')
        {
            string sArgs = strSplit(st[1],")",0,0)[0];
            list<string> args = strSplit(sArgs,",",0,0);
            for(string I : args)
            {
                list<string> at = strSplit(I," ",0,0);
                stripList(at);
                u64 argType = getType(at[0]);
                string argName = at[1];
                f.args.push_back(var(argName,0,0,argType,_variable));
            }
        }
        if(objExists(fName,f.args))
        {
            std::cout << "[ERROR] object with name \"" << fName << "\" already exists!" << std::endl;
            exit(-104);
        }
        functions.push_back(f);
        objects.push_back(objD(_function,f.subNumber,f.number));
        std::cout << "function \"" << fName << "\" declared with type: \"" << types[T-EOO].name <<"\""<< std::endl;
        return;
    }
    //variable declaration
    if(objExists(tokens[1],noArgs))
    {
        std::cout << "[ERROR] object with name \"" << tokens[1] << "\" already exists!" << std::endl;
        exit(-104);
    }

    vars.push_back(var(tokens[1],objNumber++,vars.size(),T,_variable));
    objects.push_back(objD(vars.back().ObjType,vars.back().subNumber,vars.back().number));
    std::cout << "variable \"" << tokens[1] << "\" declared with type: \"" << types[T-EOO].name <<"\""<< std::endl;
}

void compileLine(string& code)
{
    strip(code);
    if(code[0] == '/' && code[1] == '/')
        return;
    if(cLevel == 0)
        return compileLine_topLevel(code);
}

u64* compile(string& code)
{
    scopeStack.push_back(scope());
    list<string> lines = strSplit(code,";",0,0);
    for(string& I : lines)
        compileLine(I);

    u64* instr = new u64[instructions.size()];
    for(int I = 0;I<instructions.size();I++)
        instr[I] = instructions[I];
    return instr;
}