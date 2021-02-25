project(Engine)
include(${CMAKE_SOURCE_DIR}/CMake/Functions.txt)

###################
# Engine Platform #
###################
file(GLOB ENGINE_PLATFORM_SRC
    "Engine/Platform/*.cpp"
    "Engine/Platform/*.h"
    "Engine/Platform/Generic/*.cpp"
    "Engine/Platform/Generic/*.h"
)

if(WIN32)
    file(GLOB ENGINE_PLATFORM_WINDOWS_SRC
        "Engine/Platform/Windows/*.cpp"
        "Engine/Platform/Windows/*.h"
    )

    set(ENGINE_PLATFORM_SRC
        ${ENGINE_PLATFORM_SRC}
        ${ENGINE_PLATFORM_WINDOWS_SRC}
    )
endif()

build_engine_module_function(
    module_name Engine_Platform
    module_sources ${ENGINE_PLATFORM_SRC}
)


###############
# Engine Core #
###############
file(GLOB_RECURSE ENGINE_CORE_SRC
    "Engine/Core/**.cpp"
    "Engine/Core/**.h"
)

build_engine_module_function(
    module_name Engine_Core
    module_sources ${ENGINE_CORE_SRC}
    import_modules Engine_Platform
)


##############################
# Engine Rendering Interface #
##############################
file(GLOB ENGINE_RI_SRC
    "Engine/RI/*.cpp"
    "Engine/RI/*.h"
)

if(WIN32)
    if(BUILD_D3D11)
        file(GLOB ENGINE_RI_D3D11_SRC
            "Engine/RI/D3D11/*.cpp"
            "Engine/RI/D3D11/*.h"
        )

        set(ENGINE_RI_SRC
            ${ENGINE_RI_SRC}
            ${ENGINE_RI_D3D11_SRC}
        )
    endif()
endif()

build_engine_module_function(
    module_name Engine_RI
    module_sources ${ENGINE_RI_SRC}
    import_modules Engine_Platform Engine_Core
)


################
# Engine Asset #
################
file(GLOB_RECURSE ENGINE_ASSET_SRC
    "Engine/Asset/*.cpp"
    "Engine/Asset/*.h"
)

build_engine_module_function(
    module_name Engine_Asset
    module_sources ${ENGINE_ASSET_SRC}
    import_modules Engine_Core
    third_party_libs ${THIRD_PARTY_FBX_SDK_LIBS}
)


###############
# Editor Core #
###############
file(GLOB EDITOR_CORE_SRC
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
file(GLOB_RECURSE ENGINE_SRC
    "Engine/Engine/**.cpp"
    "Engine/Engine/**.h"
)

build_engine_module_function(
    module_name Engine
    module_sources ${ENGINE_SRC}
    import_modules Engine_RI Engine_Asset Editor_Core
    third_party_libs ${THIRD_PARTY_PHYSX_SDK_LIBS} 
)


##########
# Editor #
##########
file(GLOB_RECURSE EDITOR_SRC
    "Editor/Editor/**.cpp"
    "Editor/Editor/**.h"
)

build_engine_module_function(
    module_name Editor
    module_sources ${EDITOR_SRC}
    import_modules Engine
)
