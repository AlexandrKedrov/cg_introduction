cmake_minimum_required(VERSION 3.7)
project(CG_INTRODUCTION)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64_BUILD true)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(X64_BUILD false)
endif()

set(EXTERNAL ${CMAKE_CURRENT_SOURCE_DIR}/external)

if(MSVC)
    set(SDL_INCLUDE_PATH ${EXTERNAL}/sdl_vc/include)
    set(SDL_LIBS SDL2 SDL2main)
    if(X64_BUILD)
        set(SDL_LIB_PATH ${EXTERNAL}/sdl_vc/lib/x64)
    else()
        set(SDL_LIB_PATH ${EXTERNAL}/sdl_vc/lib/x86)
    endif()
elseif(X64_BUILD)
    set(SDL_LIBS 
        mingw32
        SDL2main
        SDL2
        SDL2_image)
    set(SDL_INCLUDE_PATH ${EXTERNAL}/sdl_mingw/x64/include/SDL2)
    set(SDL_LIB_PATH ${EXTERNAL}/sdl_mingw/x64/lib)
else()
    set(SDL_LIBS 
        mingw32
        SDL2main
        SDL2
        SDL2_image)
    set(SDL_INCLUDE_PATH ${EXTERNAL}/sdl_mingw/i686/include/SDL2)
    set(SDL_LIB_PATH ${EXTERNAL}/sdl_mingw/i686/lib)
endif()

message("SDL_INCLUDE_PATH ${SDL_INCLUDE_PATH}")
message("SDL_LIB_PATH ${SDL_LIB_PATH}")

include_directories(${SDL_INCLUDE_PATH})
link_directories(${SDL_LIB_PATH})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lesson_3)