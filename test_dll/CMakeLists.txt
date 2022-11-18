cmake_minimum_required(VERSION 3.10)

# Must be before project definition
set(CMAKE_TOOLCHAIN_FILE "../cmake/toolchain-cross-mingw64-linux.cmake")

# set the project name
project(BigFiles_test
        VERSION 2.0.0 
        DESCRIPTION "Notepad++ Plugin"
        LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

# add the executable
add_executable(${PROJECT_NAME} test.cpp)

add_definitions(-D_WINDOWS 
                -D_USRDLL 
                -DWIN32 
                -DUNICODE
                -D_UNICODE
                -D_DEBUG
                -D_WIN32_WINNT=0x0600
                -DNPPPLUGINTEMPLATE_EXPORTS
                -D_CRT_NONSTDC_NO_DEPRECATE
                -D_CRT_SECURE_NO_WARNINGS
                -D_CRT_NON_CONFORMING_SWPRINTFS
                )

target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../build/lib/64bit/BigFiles.dll)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ../include
    ${CMAKE_FIND_ROOT_PATH}
    )
