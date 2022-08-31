/*############################################################################################################
# File: /Users/leonexxe/code/JSIS/src/master.h                                                               #
# Project: /Users/leonexxe/code/JSIS/src                                                                     #
# Created Date: Friday, August 26th 2022, 1:44:39 am                                                         #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                      #
############################################################################################################*/
#pragma once
#include "fixedInt.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sys/stat.h>

template<typename T>
using list = std::vector<T>;
typedef std::string string;

void preproc(std::string* fc, string code);
void preProcLine(std::string* fc, string line);
string strReplace(string& str,string target, string replacement,u64* rCounter = nullptr,bool editStringLiterals = 0);

list<string> sysLibFolders = {"/usr/local/JSIS/","C:\\JSIS\\"};
std::string outputFile = "";
std::string inputFile = "";

#include "util.cpp"
#include "preproc.cpp"
#include "compile.cpp"