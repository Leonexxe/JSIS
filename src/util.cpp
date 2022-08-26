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

bool fileExists(const string& file)
{
    std::fstream fileStream;
    fileStream.open(file);
    if (fileStream.fail()) {
        return 0;
    }
    return 1;
}

string fileGetContent(string file)
{
    std::ifstream ifs(file);
    std::string content((std::istreambuf_iterator<char>(ifs) ),
                        (std::istreambuf_iterator<char>()    ) );
    return content;
}

list<string> strSplit(std::string& str,std::string delimiter, bool includeDelimiter)
{
    list<string> r;
    std::string working;
    int dc = 0;
    for(int I = 0;I<str.size();I++)
    {
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

string strReplace(string& str,string target, string replacement,u64* rCounter = nullptr)
{
    bool TAE = 0;
    if(str.substr(str.size()-target.size(),target.size()) == target)
        TAE = 1;
    list<string> split = strSplit(str,target,0);
    *rCounter = split.size()-1;
    if(split.size() == 1)
    {
        if(TAE) 
            return split[1]+replacement;
        else
            return split[1];
    }
    string r;
    string end = split.back();
    split.pop_back();

    for(string I : split)
        r.append(I+replacement);
    r.append(end);
    return r;
}