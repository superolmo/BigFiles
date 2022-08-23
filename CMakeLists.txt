cmake_minimum_required(VERSION 3.10)

# Must be before project definition
set(CMAKE_TOOLCHAIN_FILE "cmake/toolchain-cross-mingw64-linux.cmake")

# set the project name
project(BigFiles 
        VERSION 2.0.0 
        DESCRIPTION "Notepad++ Plugin"
        LANGUAGES CXX)

#add_subdirectory(resources)
add_subdirectory(src)
