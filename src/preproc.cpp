/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/preproc.cpp                                                            #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, August 26th 2022, 1:58:00 am                                                         #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
#pragma once
#include "master.h"
list<string> includedFiles;
struct define
{
    string name;
    string value;
    define(string name,string value)
        :name(name),value(value){}
};
list<define> defined;

void ppError(string e,int n)
{
    std::cout << e << std::endl;
    exit(n);
}

list<string> PPfileStack;
void preProcLine(string* fc, string line)
{
    if(line[0] != '#')
    {
        fc->append(line);
        return;
    }
    list<string> tokens = strSplit(line," ",0,1);
    if(tokens[0] == "#define")
    {
        if(tokens.size() < 3)
            ppError("not enough arguments for #define directive",-101);
        defined.push_back(define(tokens[1],tokens[2].substr(0,tokens[2].size()-1)));//if vscode spits out an error here, just ignore it
        std::cout << "[INFO] defined: " << tokens[1] << " as " << tokens[2] << std::endl;
    }
    else if(tokens[0] == "#include")
    {
        if(tokens[1][0] == '<')
        {
            bool notFound = 1;
            for(string I : sysLibFolders)
            {
                if(fileExists(I+tokens[1].substr(1,tokens[1].size()-3)))
                {
                    PPfileStack.push_back(I+tokens[1].substr(1,tokens[1].size()-3));
                    std::cout << "[INFO] preprocessing file: " << PPfileStack.back() << std::endl;
                    preproc(fc,fileGetContent(I+tokens[1].substr(1,tokens[1].size()-3)));
                    std::cout << "continuing preprocessing on file: " << PPfileStack.back() << std::endl;
                    notFound = 0;
                    break;
                }
            }
            if(notFound)
            {
                std::cout << "[ERROR]: file not found: " << tokens[1] << std::endl;
                exit(-103);
            }

        }
        else if(tokens[1][0] == '"')
        {
            if(!fileExists("./"+tokens[1].substr(1,tokens[1].size()-3)))
            {
                std::cout << "[ERROR]: file not found: " << "./"+tokens[1].substr(1,tokens[1].size()-3) << std::endl;
                exit(-103);
            }
            PPfileStack.push_back(tokens[1].substr(1,tokens[1].size()-3));
            preproc(fc,fileGetContent("./"+tokens[1].substr(1,tokens[1].size()-3)));
            fc->append("//@file "+PPfileStack.back()+";\n");
            std::cout << "continuing preprocessing on file: " << PPfileStack.back() << std::endl;
        }
        else
        {
            std::cout << "[ERROR]: " << tokens[1] << " is not a valid argument to the #include directive!" << std::endl;
            exit(-102);
        }
    }
}

void preproc(std::string* fc, string code)
{
    std::cout << "[INFO] preprocessing file: " << PPfileStack.back() << std::endl;
    fc->append("//@file "+PPfileStack.back()+";\n");
    list<string> lines = strSplit(code,";",1,0);
    for(string I : lines)
        preProcLine(fc,I);
    PPfileStack.pop_back();
}

string placePPvars(string& code)
{
    string r = code;
    u64 rc;
    for(int I = 0;I<defined.size();I++)
    {
        r = strReplace(r,defined[I].name,defined[I].value,&rc,1);
        std::cout << "found " << rc << " use(s) of " << defined[I].name << std::endl;
    }
    return r;
}