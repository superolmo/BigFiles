
# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# the name of the target operating system
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_CROSSCOMPILING true)
set(CMAKE_SYSTEM_VERSION 10.0)
set(CMAKE_SYSTEM_PROCESSOR x86_64)


# which compilers to use for C and C++
set(CMAKE_C_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32-g++)
set(CMAKE_RANLIB ${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32-ranlib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17  -Wno-conversion-null -gstabs -mwindows -mtune=generic -DBUILD_DLL -O1")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
set(CMAKE_CXX_COMPILER_ID MSVC)
set(CMAKE_RC_COMPILER ${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32-windres)

# here is the target environment located
set(CMAKE_FIND_ROOT_PATH  /usr/${CMAKE_SYSTEM_PROCESSOR}-w64-mingw32/include)

set(CMAKE_INSTALL_PREFIX ~/Development/BigFiles/build)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
