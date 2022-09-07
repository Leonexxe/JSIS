##############################################################################################################
# File: /Users/leonexxe/code/JSIS/makefile                                                                   #
# Project: /Users/leonexxe/code/JSIS                                                                         #
# Created Date: Friday, August 26th 2022, 12:27:05 pm                                                        #
# Author: Leonexxe (Leon Marcellus Nitschke-Höfer)                                                           #
# -----                                                                                                      #
# Copyright (c) 2022 Leon Marcellus Nitschke-Höfer (Leonexxe)                                                #
# -----                                                                                                      #
# You may not remove or alter this copyright header.                                                         #
##############################################################################################################
all:
	clang++ ./src/main.cpp -o ./bin/JSIS -std=c++20 -Wno-return-type -g3