cmake_minimum_required(VERSION 3.30)
project(BD_labwork1_Bezverkhyi)

set(CMAKE_CXX_STANDARD 23)

add_executable(BD_labwork1_Bezverkhyi main.cpp
        RiderObject.cpp
        RiderObject.h
        RentalObject.cpp
        RentalObject.h
        InputFunc.cpp
        InputFunc.h
        OutputFunc.cpp
        OutputFunc.h
        Validator.cpp
        Validator.h
        DataBaseCore.h
        DataBaseCore.cpp)
