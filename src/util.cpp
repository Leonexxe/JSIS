/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/util.cpp                                                               #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, August 26th 2022, 2:07:49 am                                                         #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
#pragma once
#include "master.h"

#define TAB 11

bool strContains(char c,string& str)
{
    for(char I : str)
        if(c == I)
            return 1;
    return 0;
}

void strip(string& str)
{
    for(char I : str)
    {
        if(I == ' ' || I == '\n' || I == TAB)
            str = str.substr(1,str.size()-1);
        else
            return;
    }
}

void stripList(list<string>& l)
{
    for(string& I : l)
        strip(I);
}

bool fileExists(const string& file)
{
    std::fstream fileStream;
    fileStream.open(file);
    if (fileStream.fail()) {
        return 0;
    }
    return 1;
}

list<string> operators = {"-","+","*","/","+=","-=","*=","/=","<<",">>","<",">","<=",">=","=","==","!="};
string preFormat(string& code)
{
    string pre = code;
    for(string op : operators)
    {
        string rop = " "+op+" ";
        pre = strReplace(pre,op,rop,nullptr,1);
    }
    pre = strReplace(pre,"+  =","+=",nullptr,1);
    pre = strReplace(pre,"-  =","-=",nullptr,1);
    pre = strReplace(pre,"*  =","*=",nullptr,1);
    pre = strReplace(pre,"/  =","/=",nullptr,1);
    pre = strReplace(pre," /  / ","//",nullptr,1);

    bool hasSpace = 0;
    bool isStringLiteral = 0;
    bool specialCharsEnabled = 1;
    string r;
    for(char I : pre)
    {
        if(isStringLiteral)
        {
            r.push_back(I);
            continue;
        }

        if(I == '\\')
            specialCharsEnabled = 0;
        if(I == '"' && specialCharsEnabled)
        {
            if(isStringLiteral)
                isStringLiteral = 0;
            else
                isStringLiteral = 1;
            continue;
        }
            

        if(I == ' ')
        {
            if(!hasSpace)
            {
                r.push_back(' ');
                hasSpace=1;
            }
        }
        else if(I == TAB)
        {
            if(!hasSpace)
            {
                r.push_back(' ');
                hasSpace=1;
            }
        }
        else
        {
            r.push_back(I);
            hasSpace=0;
        }
    }
    return r;
}

string fileGetContent(string file, bool pf = 1)
{
    std::ifstream ifs(file);
    std::string content((std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>()    ) );
    if(pf)
        return preFormat(content);
    return content;
}

list<string> strSplit(std::string& str,std::string delimiter, bool includeDelimiter, bool editStringLiterals = 0)
{
    list<string> r;
    std::string working;
    int dc = 0;
    bool isStringLiteral = 0;
    for(int I = 0;I<str.size();I++)
    {
        if(editStringLiterals)
        {
            if(str[I] == '"')
            {
                working.push_back(str[I]);
                if(I == 0)
                    isStringLiteral = 1;
                else if(str[I-1] != '\\' && isStringLiteral)
                    isStringLiteral = 0;
                continue;
            }
            if(isStringLiteral)
            {
                if(str[I] != '\\')
                    working.push_back(str[I]);
                continue;
            }
        }

        working.push_back(str[I]);
        if(str[I] == delimiter[dc])
            dc++;
        else
            dc=0;
        if(dc==delimiter.size())
        {
            if(!includeDelimiter)
                working=working.substr(0,working.size()-delimiter.size());
            r.push_back(working);
            working="";
            dc=0;
        }
    }
    r.push_back(working);
    return r;
}

string strReplace(string& str,string target, string replacement,u64* rCounter,bool editStringLiterals)
{
    bool TAE = 0;
    if(str.substr(str.size()-target.size(),target.size()) == target)
        TAE = 1;
    list<string> split = strSplit(str,target,0,editStringLiterals);
    if(rCounter != nullptr)
        *rCounter = split.size()-1;
    if(split.size() == 1)
    {
        if(TAE) 
            return split[0]+replacement;
        else
            return split[0];
    }
    string r;
    string end = split.back();
    split.pop_back();

    for(string I : split)
        r.append(I+replacement);
    r.append(end);
    return r;
}