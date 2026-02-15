cmake_minimum_required(VERSION 4.0)
project(scratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
add_executable(scratch main.cpp
        code.h
        costum.h
        settings.h
        run.h
        event.h
        code1.h
)
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
