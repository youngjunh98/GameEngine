cmake_minimum_required(VERSION 3.13)
project(GameEngine)

message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} (Version ${CMAKE_SYSTEM_VERSION})")
message(STATUS "==============================")

set(CMAKE_CXX_STANDARD 14)
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID} (Version ${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "==============================")

# Platform specific
if(WIN32)
    option(BUILD_D3D11 "Build D3D11 RI" ON)

    if(BUILD_D3D11)
        add_compile_definitions(ENGINE_RI_D3D11)
    endif()
elseif(APPLE)
    set(CMAKE_OSX_ARCHITECTURES x86_64)

    enable_language(Swift)
    set(CMAKE_Swift_LANGUAGE_VERSION 5)
    message(STATUS "Swift Version: ${CMAKE_Swift_LANGUAGE_VERSION}")
    message(STATUS "Swift Compiler: ${CMAKE_Swift_COMPILER_ID} (Version ${CMAKE_Swift_COMPILER_VERSION})")
    message(STATUS "==============================")

    #enable_language(OBJCXX)
    
    option(BUILD_METAL "Build Metal RI" ON)

    if(BUILD_METAL)
        add_compile_definitions(ENGINE_RI_METAL)
    endif()
endif()

# Build options
option(BUILD_STATIC "Build Static Library")

# Output directory settings
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Binaries/${CMAKE_GENERATOR_PLATFORM}/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Binaries/${CMAKE_GENERATOR_PLATFORM}/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Builds/Binaries/${CMAKE_GENERATOR_PLATFORM}/)

# Unicode support
add_compile_definitions(UNICODE _UNICODE)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/Source)


#######
# STB #
#######
include_directories(${CMAKE_SOURCE_DIR}/Library/stb-master)


###########
# FBX SDK #
###########
add_library(libfbxsdk STATIC IMPORTED)
set(THIRD_PARTY_FBX_SDK_PATH "${CMAKE_SOURCE_DIR}/Library/FBX\ SDK\ 2019.0")
set(THIRD_PARTY_FBX_SDK_INCLUDE "${THIRD_PARTY_FBX_SDK_PATH}/include")
set(THIRD_PARTY_FBX_SDK_LIBS libfbxsdk)

if(WIN32)
    link_directories(${THIRD_PARTY_FBX_SDK_PATH}/lib/vs2015/${CMAKE_GENERATOR_PLATFORM}/${CMAKE_BUILD_TYPE})
    set(THIRD_PARTY_FBX_SDK_LIBS libfbxsdk-md)
elseif(APPLE)
    target_link_libraries(libfbxsdk INTERFACE "-framework CoreFoundation")

    set_target_properties(libfbxsdk PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_FBX_SDK_PATH}/lib/clang/debug/libfbxsdk.a)
    set_target_properties(libfbxsdk PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${THIRD_PARTY_FBX_SDK_PATH}/lib/clang/debug/libfbxsdk.a)
    set_target_properties(libfbxsdk PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_FBX_SDK_PATH}/lib/clang/release/libfbxsdk.a)
    set_target_properties(libfbxsdk PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${THIRD_PARTY_FBX_SDK_PATH}/lib/clang/release/libfbxsdk.a)
endif()


#############
# PhysX SDK #
#############
add_library(PhysX_static STATIC IMPORTED)
add_library(PhysXCommon_static STATIC IMPORTED)
add_library(PhysXExtensions_static STATIC IMPORTED)
add_library(PhysXFoundation_static STATIC IMPORTED)
add_library(PhysXPvdSDK_static STATIC IMPORTED)

set(THIRD_PARTY_PHYSX_SDK_PATH "${CMAKE_SOURCE_DIR}/Library/PhysX\ 4.1")
set(THIRD_PARTY_PHYSX_SDK_INCLUDE "${THIRD_PARTY_PHYSX_SDK_PATH}/physx/include/" "${THIRD_PARTY_PHYSX_SDK_PATH}/pxshared/include")
set(THIRD_PARTY_PHYSX_SDK_LIBS PhysX_static PhysXCommon_static PhysXExtensions_static PhysXFoundation_static PhysXPvdSDK_static)

if(WIN32)
    if(${CMAKE_GENERATOR_PLATFORM} STREQUAL "x32")
        set(THIRD_PARTY_PHYSX_SDK_PLATFORM 32)
    elseif(${CMAKE_GENERATOR_PLATFORM} STREQUAL "x64")
        set(THIRD_PARTY_PHYSX_SDK_PLATFORM 64)
    endif()

    link_directories(${THIRD_PARTY_PHYSX_SDK_PATH}/bin/win.x86_${THIRD_PARTY_PHYSX_SDK_PLATFORM}.vc141.md/${CMAKE_BUILD_TYPE})
    set(THIRD_PARTY_PHYSX_SDK_LIBS
        PhysX_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
        PhysXCommon_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
        PhysXExtensions_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
        PhysXFoundation_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
        PhysXPvdSDK_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
    )
elseif(APPLE)
    set_target_properties(PhysX_static PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysX_static_64.a)
    set_target_properties(PhysX_static PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysX_static_64.a)
    set_target_properties(PhysX_static PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysX_static_64.a)
    set_target_properties(PhysX_static PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysX_static_64.a)

    set_target_properties(PhysXCommon_static PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXCommon_static_64.a)
    set_target_properties(PhysXCommon_static PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXCommon_static_64.a)
    set_target_properties(PhysXCommon_static PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXCommon_static_64.a)
    set_target_properties(PhysXCommon_static PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXCommon_static_64.a)

    set_target_properties(PhysXExtensions_static PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXExtensions_static_64.a)
    set_target_properties(PhysXExtensions_static PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXExtensions_static_64.a)
    set_target_properties(PhysXExtensions_static PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXExtensions_static_64.a)
    set_target_properties(PhysXExtensions_static PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXExtensions_static_64.a)

    set_target_properties(PhysXFoundation_static PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXFoundation_static_64.a)
    set_target_properties(PhysXFoundation_static PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXFoundation_static_64.a)
    set_target_properties(PhysXFoundation_static PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXFoundation_static_64.a)
    set_target_properties(PhysXFoundation_static PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXFoundation_static_64.a)

    set_target_properties(PhysXPvdSDK_static PROPERTIES IMPORTED_LOCATION_DEBUG ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXPvdSDK_static_64.a)
    set_target_properties(PhysXPvdSDK_static PROPERTIES IMPORTED_LOCATION_RELWITHDEBINFO ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/debug/libPhysXPvdSDK_static_64.a)
    set_target_properties(PhysXPvdSDK_static PROPERTIES IMPORTED_LOCATION_RELEASE ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXPvdSDK_static_64.a)
    set_target_properties(PhysXPvdSDK_static PROPERTIES IMPORTED_LOCATION_MINSIZEREL ${THIRD_PARTY_PHYSX_SDK_PATH}/physx/bin/mac.x86_64/release/libPhysXPvdSDK_static_64.a)
endif()


#######################
# JSON for Modern C++ #
#######################
include_directories(${CMAKE_SOURCE_DIR}/Library/json-3.7.3)


##############
# Dear ImGui #
##############
set(THIRD_PARTY_DEAR_IMGUI_PATH "${CMAKE_SOURCE_DIR}/Library/imgui-1.76/")
include_directories(${THIRD_PARTY_DEAR_IMGUI_PATH})
include_directories(${THIRD_PARTY_DEAR_IMGUI_PATH}/examples)


#############################
# Executable Build Settings #
#############################
add_subdirectory(Source)

if(WIN32)
    add_executable(Executable WIN32 "Source/Main/WinMain.cpp")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Executable)
elseif(APPLE)
    file(GLOB MAC_EXECUTABLE_SRC "Source/Main/Mac/*.swift")
    add_executable(Executable ${MAC_EXECUTABLE_SRC})
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY Executable)
endif()

set_target_properties(Executable PROPERTIES OUTPUT_NAME "GameEngine")
add_dependencies(Executable Engine Editor)

target_compile_definitions(Executable PRIVATE IMPORT_ENGINE IMPORT_EDITOR)
target_link_libraries(Executable PRIVATE Engine Editor)
