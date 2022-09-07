/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/main.cpp                                                               #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, August 26th 2022, 1:18:32 am                                                         #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
############################################################################################################*/
void showASM();
#include "master.h"

bool isInvalidChar(char c)
{
    if(
        c == '#' ||
        c == '-'
    )return 1;
    return 0;
}

i64 parseFlags(int argc, char** argv)
{
    if(argc < 2)
    {
        std::cout << "error: not enough arguments to run the JSIS compiler!" << std::endl;
        return -1;
    }

    for(int I = 1;I<argc;I++)
    {
        if(argv[I][0] == '-')
        {
            if(argv[I][1] == 'o')
            {
                if(!isInvalidChar(argv[I+1][0]))
                {
                    outputFile = argv[I+1];
                }
                else
                {
                    std::cout << "error: \"" << argv[I+1] << "\" is not a valid argument to the -o option!" << std::endl;
                    return -1;
                }
            }
            else if(argv[I][1] == 'D')
                f_Debug = true;
            else if(argv[I][1] == 'V')
                f_Verbose = true;
            else if(argv[I][1] == 'A')
                f_Assembly = true;
            else if(string("-assembler") == argv[I])
                f_Compile  = false;
        }
        else
        {
            inputFile = argv[I];
        }
    }
    return 0;
}

int main(int argc, char** argv)
{
    if(i64 PF = parseFlags(argc,argv);PF!=0)
        return PF;

    std::cout << "running JSIS compiler..." << std::endl;
    std::cout << "inputFile: "  << inputFile  << std::endl;
    std::cout << "outputFile: " << outputFile << std::endl;

    string initCode = fileGetContent(inputFile);
    string code;

    std::cout << "[INFO] running preprocessor..." << std::endl;
    PPfileStack.push_back(inputFile);
    currentFile = inputFile;
    preproc(&code,initCode);
    std::cout << "[INFO] placing in preprocessor variables..." << std::endl;
    string fCode = placePPvars(code);
    std::cout << "final code " << std::endl << fCode << std::endl;
    if(f_Compile)
    {
        std::cout << "running compiler..." << std::endl;
        compile(fCode);
    }
    else
    {
        std::cout << "running assembler..." << std::endl;
        goto assOnly;
    }

    std::cout << "generated assembly code:" << std::endl << assembly << std::endl;

    if(!f_Assembly)
        assemble();
    else
        writeAsm();
    goto end;
    assOnly: ;
    assembly=fCode;
    assemble();

    end: ;
    return 0;
}

void showASM()
{
    std::cout << "generated assembly code:" << std::endl << assembly << std::endl;
    exit(1);
}