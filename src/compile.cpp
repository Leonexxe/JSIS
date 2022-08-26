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
#include <master.h>

enum objType
{
    _type,_variable,_function
};

struct obj
{
    string name;
    u64 number;
    u64 subNumber;
    objType ObjType;

    obj(string name,u64 number,u64 subNumber,objType ObjType)
        :name(name),number(number),subNumber(subNumber),ObjType(ObjType){}
};

struct type : public obj
{
    using obj::obj;
    list<function*> memberFunctions;
    list<var*> memberVariables;
};

struct function : public obj
{
    using obj::obj;
};

struct var : public obj
{
    using obj::obj;
};

list<function>  functions;
list<type>      types;
list<var>       vars;
list<obj*>      objects;

u64 objNumber = 0;

type* getType(string name)
{
    for(int I = 0;I<types.size();I++)
        if(types[I].name == name)
            return &types[I];
    std::cout << "[ERROR] type not found: " << name << std::endl;
    exit(1);
}

u64* compile(string& code)
{
    list<string> tokens = strSplit(code,";",0);
    if(tokens[0] == "class")
    {
        //type
        types.push_back(type(
                            tokens[1],
                            objNumber++,
                            (u64)types.size(),
                            _type
                            ));
    }
    type* T = getType(tokens[0]);
    std::cout << tokens[1] << std::endl;
    std::cout << tokens[2] << std::endl;
    std::cout << tokens[3] << std::endl;
}