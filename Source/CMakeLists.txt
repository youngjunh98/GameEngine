project(Engine)

############
# Platform #
############
file(GLOB PLATFORM_SRC
    "Platform/*.cpp"
    "Platform/*.h"
    "Platform/Generic/*.cpp"
    "Platform/Generic/*.h"
)

if(WIN32)
    file(GLOB WINDOWS_PLATFORM_SRC
        "Platform/Windows/*.cpp"
        "Platform/Windows/*.h"
    )

    set(PLATFORM_SRC
        ${PLATFORM_SRC}
        ${WINDOWS_PLATFORM_SRC}
    )
endif()

if(BUILD_STATIC)
    add_library(Platform STATIC ${PLATFORM_SRC})
else()
    add_library(Platform SHARED ${PLATFORM_SRC})
endif()

set_target_properties(Platform PROPERTIES OUTPUT_NAME "Engine_Platform")
target_compile_definitions(Platform PRIVATE EXPORT_PLATFORM)


########
# Core #
########
file(GLOB_RECURSE CORE_SRC
    "Core/**.cpp"
    "Core/**.h"
)

if(BUILD_STATIC)
    add_library(Core STATIC ${CORE_SRC})
else()
    add_library(Core SHARED ${CORE_SRC})
endif()

set_target_properties(Core PROPERTIES OUTPUT_NAME "Engine_Core")
add_dependencies(Core Platform)

target_compile_definitions(Core PRIVATE EXPORT_CORE)

target_compile_definitions(Core PUBLIC IMPORT_PLATFORM)
target_link_libraries(Core PUBLIC Platform)


#######################
# Rendering Interface #
#######################
file(GLOB RI_SRC
    "RI/*.cpp"
    "RI/*.h"
)

if(WIN32)
    if(BUILD_RI_D3D11)
        file(GLOB D3D11_SRC
            "RI/D3D11/*.cpp"
            "RI/D3D11/*.h"
        )

        set(RI_SRC
            ${RI_SRC}
            ${D3D11_SRC}
        )
    endif()
endif()

if(BUILD_STATIC)
    add_library(RI STATIC ${RI_SRC})
else()
    add_library(RI SHARED ${RI_SRC})
endif()

set_target_properties(RI PROPERTIES OUTPUT_NAME "Engine_RI")
add_dependencies(RI Platform Core)

target_compile_definitions(RI PRIVATE EXPORT_RI)

target_compile_definitions(RI PUBLIC IMPORT_PLATFORM IMPORT_CORE)
target_link_libraries(RI PUBLIC Platform Core)


##########
# Engine #
##########
file(GLOB_RECURSE ENGINE_SRC
    "Engine/**.cpp"
    "Engine/**.h"
)

file(GLOB_RECURSE EDITOR_SRC
    "Editor/**.cpp"
    "Editor/**.h"
)

if(BUILD_STATIC)
    add_library(Engine STATIC
        ${ENGINE_SRC}
        ${EDITOR_SRC}
    )
else()
    add_library(Engine SHARED
        ${ENGINE_SRC}
        ${EDITOR_SRC}
    )
endif()

set_target_properties(Engine PROPERTIES OUTPUT_NAME "Engine")
add_dependencies(Engine Core RI)

target_compile_definitions(Engine PRIVATE EXPORT_ENGINE)
target_link_libraries(Engine PRIVATE ${THIRD_PARTY_FBX_SDK_LIBS} ${THIRD_PARTY_PHYSX_SDK_LIBS})

target_compile_definitions(Engine PUBLIC IMPORT_CORE IMPORT_RI)
target_link_libraries(Engine PUBLIC Core RI)
