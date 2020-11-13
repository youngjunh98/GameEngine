cmake_minimum_required(VERSION 3.13)
project(GameEngine)

# Build options
option(BUILD_STATIC "Build Static Library")
option(BUILD_RI_D3D11 "Build D3D11 RI")

# Output directory settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Binaries/${CMAKE_GENERATOR_PLATFORM}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Binaries/${CMAKE_GENERATOR_PLATFORM}/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Binaries/${CMAKE_GENERATOR_PLATFORM}/)

# Unicode support
add_compile_definitions(UNICODE _UNICODE)

# Game engine macros
if(BUILD_RI_D3D11)
    add_compile_definitions(RI_D3D11)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Engine)
include_directories(${CMAKE_SOURCE_DIR}/Editor/ImGui/)


#######
# STB #
#######

include_directories(${CMAKE_SOURCE_DIR}/Library/stb-master)


###########
# FBX SDK #
###########

set(THIRD_PARTY_FBX_SDK_PATH "${CMAKE_SOURCE_DIR}/Library/FBX\ SDK\ 2019.0")
include_directories(${THIRD_PARTY_FBX_SDK_PATH}/include)
link_directories(${THIRD_PARTY_FBX_SDK_PATH}/lib/vs2015/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})


#############
# PhysX SDK #
#############

set(THIRD_PARTY_PHYSX_SDK_PATH "${CMAKE_SOURCE_DIR}/Library/PhysX\ 4.1/physx")
set(THIRD_PARTY_PHYSX_SDK_PXSHARED_PATH "${CMAKE_SOURCE_DIR}/Library/PhysX\ 4.1/pxshared")
include_directories(${THIRD_PARTY_PHYSX_SDK_PATH}/include/)
include_directories(${THIRD_PARTY_PHYSX_SDK_PXSHARED_PATH}/include)

if(${CMAKE_GENERATOR_PLATFORM} STREQUAL "x32")
    set(THIRD_PARTY_PHYSX_SDK_PLATFORM "32")

elseif(${CMAKE_GENERATOR_PLATFORM} STREQUAL "x64")
    set(THIRD_PARTY_PHYSX_SDK_PLATFORM "64")
endif()

set(THIRD_PARTY_PHYSX_SDK_LIB_PATH "${THIRD_PARTY_PHYSX_SDK_PATH}/bin/win.x86_${THIRD_PARTY_PHYSX_SDK_PLATFORM}.vc141.md/${CMAKE_BUILD_TYPE}")
link_directories(${THIRD_PARTY_PHYSX_SDK_LIB_PATH})


#######################
# JSON for Modern C++ #
#######################

include_directories(${CMAKE_SOURCE_DIR}/Library/json-3.7.3)


#############################
# Executable Build Settings #
#############################
add_subdirectory(Engine)

if(WIN32)
    add_executable(Executable WIN32 "${CMAKE_SOURCE_DIR}/Engine/Platform/Windows/WinMain.cpp")
endif()

add_dependencies(Executable
    Engine
)

target_compile_definitions(Executable PRIVATE
    IMPORT_ENGINE
)

target_link_libraries(Executable
    Engine
)

set_target_properties(Executable PROPERTIES OUTPUT_NAME "GameEngine")
