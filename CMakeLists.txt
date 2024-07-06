cmake_minimum_required(VERSION 3.9.6...3.15.0)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15 CACHE STRING "Build for 10.15")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(SERVER_PLUGIN TuningServer)
set(SERVER_CLI tuning-server)
set(CLIENT_CLI tuning-client)
set(TESTS tests)

project(TuningServer LANGUAGES CXX)


# Server command line executable

add_executable(
    ${SERVER_CLI}
    src/server.cpp
    libs/MTS-ESP/Client/libMTSClient.cpp
)

target_include_directories(
    ${SERVER_CLI}
    PUBLIC
    libs/cpp-httplib
    libs/MTS-ESP/Client
)


# Client command line executable

add_executable(
    ${CLIENT_CLI}
    src/client.cpp
    libs/MTS-ESP/Master/libMTSMaster.cpp
)

target_include_directories(
    ${CLIENT_CLI}
    PUBLIC
    libs/cpp-httplib
    libs/MTS-ESP/Master
)


# Server CLAP plugin

add_library(
    ${SERVER_PLUGIN}
    MODULE
    src/server_plugin.cpp
    libs/MTS-ESP/Client/libMTSClient.cpp
)

set_target_properties(${SERVER_PLUGIN} PROPERTIES SUFFIX ".clap" PREFIX "")

target_include_directories(
    ${SERVER_PLUGIN}
    PUBLIC
    libs/cpp-httplib
    libs/MTS-ESP/Client
    libs/clap/include
)


# Tests

add_executable(
    ${TESTS}
    tests/tests.cpp
    libs/MTS-ESP/Client/libMTSClient.cpp
)

target_include_directories(
    ${TESTS}
    PUBLIC
    src
    libs/MTS-ESP/Client
    libs/cpp-httplib
)
