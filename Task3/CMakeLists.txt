cmake_minimum_required(VERSION 3.12)
project(Task3 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Task3
        analyze.c
        analyze.h
        bitmap_types.h
        error.h
        input.c
        input.h
        list.c
        list.h
        main.c
        ms_types.h
        output.c
        output.h
        packed.h
        vlarray.c
        vlarray.h)
