project(Engine)
include(${CMAKE_SOURCE_DIR}/CMake/Functions.cmake)
include(${CMAKE_SOURCE_DIR}/CMake/AppleHelper.cmake)

###################
# Engine Platform #
###################
search_and_append_files(ENGINE_PLATFORM_SRC search_paths
    "Engine/Platform/*.cpp"
    "Engine/Platform/*.h"
    "Engine/Platform/Generic/*.cpp"
    "Engine/Platform/Generic/*.h"
)

search_and_append_files(ENGINE_PLATFORM_SRC search_paths
    "Engine/Platform/Windows/*.cpp"
    "Engine/Platform/Windows/*.h"
    if WIN32
)

search_and_append_files(ENGINE_PLATFORM_SRC search_paths
    "Engine/Platform/Mac/*.cpp"
    "Engine/Platform/Mac/*.h"
    "Apple/Platform/*.swift"
    "Apple/Platform/*.mm"
    "Apple/Platform/*.m"
    "Apple/Platform/*.h"
    if APPLE
)

build_engine_module_function(
    module_name Engine_Platform
    module_sources ${ENGINE_PLATFORM_SRC}
)

set_swift_bridging_header(
    target Engine_Platform
    header_path "${CMAKE_SOURCE_DIR}/Source/Apple/Platform/Bridging-Header.h"
)


###############
# Engine Core #
###############
search_and_append_files(ENGINE_CORE_SRC search_paths
    "Engine/Core/*.cpp"
    "Engine/Core/*.h"
    "Engine/Core/Application/*.cpp"
    "Engine/Core/Application/*.h"
    "Engine/Core/File/*.cpp"
    "Engine/Core/File/*.h"
    "Engine/Core/Input/*.cpp"
    "Engine/Core/Input/*.h"
    "Engine/Core/JSON/*.cpp"
    "Engine/Core/JSON/*.h"
    "Engine/Core/Math/*.cpp"
    "Engine/Core/Math/*.h"
    "Engine/Core/Modular/*.cpp"
    "Engine/Core/Modular/*.h"
    "Engine/Core/Object/*.cpp"
    "Engine/Core/Object/*.h"
    "Engine/Core/Time/*.cpp"
    "Engine/Core/Time/*.h"
)

search_and_append_files(ENGINE_CORE_SRC search_paths
    "Engine/Core/Modular/Platform/WindowsImportDLL.cpp"
    if WIN32
)

search_and_append_files(ENGINE_CORE_SRC search_paths
    "Engine/Core/Modular/Platform/MacImportDL.cpp"
    if APPLE
)

build_engine_module_function(
    module_name Engine_Core
    module_sources ${ENGINE_CORE_SRC}
    import_modules Engine_Platform
)


##############################
# Engine Rendering Interface #
##############################
search_and_append_files(ENGINE_RI_SRC search_paths
    "Engine/RI/*.cpp"
    "Engine/RI/*.h"
)

search_and_append_files(ENGINE_RI_SRC search_paths
    "Engine/RI/D3D11/*.cpp"
    "Engine/RI/D3D11/*.h"
    if WIN32
)

search_and_append_files(ENGINE_RI_SRC search_paths
    "Engine/RI/Metal/*.cpp"
    "Engine/RI/Metal/*.h"
    "Apple/Metal/*.swift"
    "Apple/Metal/*.mm"
    "Apple/Metal/*.m"
    "Apple/Metal/*.h"
    if APPLE
)

build_engine_module_function(
    module_name Engine_RI
    module_sources ${ENGINE_RI_SRC}
    import_modules Engine_Platform Engine_Core
)


################
# Engine Asset #
################
search_and_append_files(ENGINE_ASSET_SRC recursive_mode search_paths
    "Engine/Asset/*.cpp"
    "Engine/Asset/*.h"
)

build_engine_module_function(
    module_name Engine_Asset
    module_sources ${ENGINE_ASSET_SRC}
    import_modules Engine_Core
    third_party_libs ${THIRD_PARTY_FBX_SDK_LIBS}
    third_party_include_paths ${THIRD_PARTY_FBX_SDK_INCLUDE}
)

###############
# Editor Core #
###############
search_and_append_files(EDITOR_CORE_SRC search_paths
    "Editor/Core/*.cpp"
    "Editor/Core/*.h"
)

build_engine_module_function(
    module_name Editor_Core
    module_sources ${EDITOR_CORE_SRC}
    import_modules Engine_RI Engine_Asset
)


##########
# Engine #
##########
search_and_append_files(ENGINE_SRC recursive_mode search_paths
    "Engine/Engine/**.cpp"
    "Engine/Engine/**.h"
)

build_engine_module_function(
    module_name Engine
    module_sources ${ENGINE_SRC}
    import_modules Engine_RI Engine_Asset Editor_Core
    third_party_libs ${THIRD_PARTY_PHYSX_SDK_LIBS}
    third_party_include_paths ${THIRD_PARTY_PHYSX_SDK_INCLUDE}
)


##########
# Editor #
##########
search_and_append_files(EDITOR_SRC recursive_mode search_paths
    "Editor/Editor/**.cpp"
    "Editor/Editor/**.h"
)

build_engine_module_function(
    module_name Editor
    module_sources ${EDITOR_SRC}
    import_modules Engine
)
