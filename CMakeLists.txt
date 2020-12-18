cmake_minimum_required(VERSION 3.17)
project(OOP2_Basic_interpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(OOP2_Basic_interpreter Basic/Basic.cpp
        Basic/evalstate.h Basic/evalstate.cpp
        Basic/exp.h Basic/exp.cpp
        Basic/parser.h Basic/parser.cpp
        Basic/program.h Basic/program.cpp
        Basic/statement.h Basic/statement.cpp
        StanfordCPPLib/error.h StanfordCPPLib/error.cpp
        StanfordCPPLib/strlib.h StanfordCPPLib/strlib.cpp
        StanfordCPPLib/tokenscanner.h StanfordCPPLib/tokenscanner.cpp
        StanfordCPPLib/map.h
        StanfordCPPLib/simpio.h StanfordCPPLib/simpio.cpp
        )