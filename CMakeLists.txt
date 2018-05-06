cmake_minimum_required(VERSION 3.9)
project(SimulatorStarterCode)

set(CMAKE_CXX_STANDARD 11)

add_executable(SimulatorStarterCode
        bstr.c
        bstr.h
        comp.c
        comp.h
        simulator.c)
