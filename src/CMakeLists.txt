cmake_minimum_required(VERSION 3.14)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

set(BigFiles_SRC 
    Configuration.cpp
    FileTracker.cpp
    libmagic_alike.cpp
    PluginDefinition.cpp
    Support.cpp
    BigFiles.cpp
    v3.cpp
    )

# Resource
# See: https://gitlab.kitware.com/cmake/community/-/wikis/doc/tutorials/How-To-Write-Platform-Checks
if(WIN32)
    if(MINGW)
        set_source_files_properties(${CMAKE_SOURCE_DIR}/resources/BigFiles.rc LANGUAGE RC)
        list(APPEND BigFiles_SRC ${CMAKE_SOURCE_DIR}/resources/BigFiles.rc)
        ENABLE_LANGUAGE(RC)
        set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -o <OBJECT> <SOURCE>")
        set(RC_CFLAGS "-DMINGW -Ocoff")
        set_source_files_properties(${CMAKE_SOURCE_DIR}/resources/BigFiles.rc COMPILE_FLAGS "${RC_CFLAGS}")
    endif(MINGW)
endif(WIN32)

if(${CMAKE_SYSTEM_PROCESSOR} STREQUAL "i686")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/32bit)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/32bit)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/32bit)
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/64bit)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/64bit)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/64bit)
endif()

file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${BigFiles_SOURCE_DIR}/include/*.h")

# add the executable
add_library(${PROJECT_NAME} SHARED ${BigFiles_SRC} ${HEADER_LIST})

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".dll")

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

target_include_directories(${PROJECT_NAME} PUBLIC 
    ../include
    ${CMAKE_FIND_ROOT_PATH}
    )

target_link_libraries(${PROJECT_NAME}
    shlwapi
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        PREFIX ""
        SUFFIX ""
        LINK_FLAGS "-Wl,--add-stdcall-alias -static -static-libgcc -static-libstdc++ -s -os ${CFLAGS}"
        POSITION_INDEPENDENT_CODE 0 # this is to avoid MinGW warning; MinGW generates position-independent-code for DLL by default
        )

file(COPY ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

#file(COPY /usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/libstdc++-6.dll DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#file(COPY /usr/lib/gcc/x86_64-w64-mingw32/9.3-win32/libgcc_s_seh-1.dll DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
