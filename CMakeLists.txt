cmake_minimum_required(VERSION 3.25)
project(SGD_projekt)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_INCLUDE_DIR C:/LibSDL/VC/SDL2/include)
set(SDL2_LIB_DIR C:/LibSDL/VC/SDL2/lib/x64)

set(SDL2MIXER_INCLUDE_DIR C:/LibSDL/VC/SDL2_mix/include)
set(SDL2MIXER_LIB_DIR C:/LibSDL/VC/SDL2_mix/lib/x64)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp player_c.cpp player_c.h vec2d.cpp vec2d.h)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIB_DIR}/SDL2.lib ${SDL2_LIB_DIR}/SDL2main.lib ${SDL2MIXER_LIB_DIR}/SDL2_mixer.lib)