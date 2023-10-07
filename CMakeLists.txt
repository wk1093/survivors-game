cmake_minimum_required(VERSION 3.26)
project(survivors LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(survivors src/main.cpp
        src/engine/engine.h
        src/engine/atlas.h
        src/engine/all.h
        src/engine/all_sf.h
        src/game/Entity.h
        src/game/Object.h)
target_link_libraries(survivors PRIVATE sfml-graphics)

if (WIN32)
    add_custom_command(
            TARGET survivors
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:survivors>
            VERBATIM)
endif()

install(TARGETS survivors)
