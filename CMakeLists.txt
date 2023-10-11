cmake_minimum_required(VERSION 3.18)
project(survivors LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(survivors src/main.cpp
        src/gfx/Engine.h
        src/gfx/TextureAtlas.h
        src/gfx/all.h
        src/gfx/all_sf.h
        src/ecs/Object.h
        src/ecs/EntityComponentSystem.h
        src/ecs/Component.h
        src/ecs/Components.h
        src/ecs/all.h
        src/ecs/Objects.h
        src/test/wasd.h
        src/test/focused_camera.h
        src/map/Map.h
        src/map/Editor.h
        src/map/MapGenerator.h
        src/map/all.h)
target_link_libraries(survivors PRIVATE sfml-graphics)

if (WIN32)
    add_custom_command(
            TARGET survivors
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:survivors>
            VERBATIM)
endif()

install(TARGETS survivors)
