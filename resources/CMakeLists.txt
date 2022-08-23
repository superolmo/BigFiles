ENABLE_LANGUAGE(RC)

set(SOURCE BigFiles.rc)
set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -i <SOURCE>")
set(CMAKE_RC_CREATE_SHARED_LIBRARY resources)
set(CMAKE_RC_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_FIND_ROOT_PATH})

# add the executable
add_library(resources SHARED ${SOURCE})

target_include_directories(resources PUBLIC 
    ../include
    )

