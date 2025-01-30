cmake_minimum_required(VERSION 3.16)
project(Pang LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)



set(PROJECT_HEADERS 
    include/game.h
    include/world.h
    include/player.h
    include/harpoon.h
    include/ball.h
    include/pooler.h
    include/passive.h
)

set(PROJECT_SOURCES 
    src/main.cpp
    src/game.cpp
    src/world.cpp
    src/player.cpp
    src/harpoon.cpp
    src/ball.cpp
    src/pooler.cpp
    src/passive.cpp
)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

target_link_libraries(Pang PRIVATE sfml-graphics)
target_compile_features(Pang PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET Pang
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Pang>
        VERBATIM)
endif()

install(TARGETS Pang)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
