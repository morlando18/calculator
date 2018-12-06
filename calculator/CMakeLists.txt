cmake_minimum_required(VERSION 3.3)
project(FinalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES

    Compute.cpp
    Compute.h
    Exponential.cpp
    Exponential.h
    Expression.cpp
    Expression.h
    Integer.cpp
    Integer.h
    Logarithm.cpp
    Logarithm.h
    main.cpp
    Expressions.h
    Rational.cpp
    Rational.h
    Transcendental.cpp
    Transcendental.h)

add_executable(FinalProject ${SOURCE_FILES})