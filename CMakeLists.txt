cmake_minimum_required(VERSION 3.14)

# Must be before project definition
# set(CMAKE_TOOLCHAIN_FILE "cmake/toolchain-cross-mingw32-linux.cmake")

set(CMAKE_CXX_COMPILER_WORKS 1)

# set the project name
project(BigFiles.dll 
        VERSION 2.0.0 
        DESCRIPTION "Notepad++ Plugin"
        LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Let's nicely support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Testing only available if this is the main app
  # Note this needs to be done in the main CMakeLists
  # since it calls enable_testing, which must be in the
  # main CMakeLists.
  include(CTest)

  # Docs only available if this is the main app
  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()

endif()

#add_subdirectory(resources)
add_subdirectory(src)
