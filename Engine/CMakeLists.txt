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

    # Remove WinMain from windows platform source files
    list(REMOVE_ITEM WINDOWS_PLATFORM_SRC
        "${PROJECT_SOURCE_DIR}/Platform/Windows/WinMain.cpp"
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

target_compile_definitions(Platform PRIVATE
    EXPORT_PLATFORM
)

set_target_properties(Platform PROPERTIES OUTPUT_NAME "Engine_Platform")


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

target_compile_definitions(Core PRIVATE
    EXPORT_CORE
)

target_compile_definitions(Core PUBLIC
    IMPORT_PLATFORM
)

target_link_libraries(Core PUBLIC
    Platform
)

set_target_properties(Core PROPERTIES OUTPUT_NAME "Engine_Core")


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

add_dependencies(RI
    Platform    
    Core    
)

target_compile_definitions(RI PRIVATE
    EXPORT_RI
)

target_compile_definitions(RI PUBLIC
    IMPORT_PLATFORM
    IMPORT_CORE
)

target_link_libraries(RI PUBLIC
    Platform
    Core
)

set_target_properties(RI PROPERTIES OUTPUT_NAME "Engine_RI")


##########
# Engine #
##########

file(GLOB ENGINE_SRC
    "*.cpp"
    "*.h"
)

file(GLOB_RECURSE RENDERING_SRC
    "Rendering/**.cpp"
    "Rendering/**.h"
)

file(GLOB_RECURSE COMPONENT_SRC
    "Component/**.cpp"
    "Component/**.h"
)

file(GLOB_RECURSE SCENE_SRC
    "Scene/**.cpp"
    "Scene/**.h"
)

file(GLOB_RECURSE ASSET_SRC
    "Asset/**.cpp"
    "Asset/**.h"
)

file(GLOB_RECURSE AUDIO_SRC
    "Audio/**.cpp"
    "Audio/**.h"
)

file(GLOB_RECURSE PHYSICS_SRC
    "Physics/**.cpp"
    "Physics/**.h"
)

file(GLOB IMGUI_SRC
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/*.h"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/examples/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/examples/*.h"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/misc/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/misc/*.h"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/misc/cpp/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/ImGui/misc/cpp/*.h"
)

file(GLOB EDITOR_SRC
    "${CMAKE_SOURCE_DIR}/Editor/*.cpp"
    "${CMAKE_SOURCE_DIR}/Editor/*.h"
)

if(BUILD_STATIC)
    add_library(Engine STATIC
        ${ENGINE_SRC}
        ${RENDERING_SRC}
        ${COMPONENT_SRC}
        ${SCENE_SRC}
        ${ASSET_SRC}
        ${AUDIO_SRC}
        ${PHYSICS_SRC}
        ${IMGUI_SRC}
        ${EDITOR_SRC}
    )
else()
    add_library(Engine SHARED
        ${ENGINE_SRC}
        ${RENDERING_SRC}
        ${COMPONENT_SRC}
        ${SCENE_SRC}
        ${ASSET_SRC}
        ${AUDIO_SRC}
        ${PHYSICS_SRC}
        ${IMGUI_SRC}
        ${EDITOR_SRC}
    )
endif()

add_dependencies(Engine
    Core
    RI
)

target_compile_definitions(Engine PRIVATE
    EXPORT_ENGINE
)

target_compile_definitions(Engine PUBLIC
    IMPORT_CORE
    IMPORT_RI
)

target_link_libraries(Engine PRIVATE
    libfbxsdk-md
    PhysX_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
    PhysXCommon_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
    PhysXExtensions_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
    PhysXFoundation_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
    PhysXPvdSDK_static_${THIRD_PARTY_PHYSX_SDK_PLATFORM}
)

target_link_libraries(Engine PUBLIC
    Core
    RI
)

set_target_properties(Engine PROPERTIES OUTPUT_NAME "Engine")
