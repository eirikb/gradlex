project(gg C)

set(CMAKE_C_STANDARD 99)

if (WIN32)
    add_executable(gg main-win.c)
else ()
    add_executable(gg main.c)
endif ()