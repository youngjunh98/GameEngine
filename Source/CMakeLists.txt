project(Engine)

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

if(BUILD_STATIC)
    add_library(EnginePlatform STATIC ${ENGINE_PLATFORM_SRC})
else()
    add_library(EnginePlatform SHARED ${ENGINE_PLATFORM_SRC})
endif()

set_target_properties(EnginePlatform PROPERTIES OUTPUT_NAME "Engine_Platform")
target_compile_definitions(EnginePlatform PRIVATE EXPORT_ENGINE_PLATFORM)


###############
# Engine Core #
###############
file(GLOB_RECURSE ENGINE_CORE_SRC
    "Engine/Core/**.cpp"
    "Engine/Core/**.h"
)

if(BUILD_STATIC)
    add_library(EngineCore STATIC ${ENGINE_CORE_SRC})
else()
    add_library(EngineCore SHARED ${ENGINE_CORE_SRC})
endif()

set_target_properties(EngineCore PROPERTIES OUTPUT_NAME "Engine_Core")
target_compile_definitions(EngineCore PRIVATE EXPORT_ENGINE_CORE)
target_link_libraries(EngineCore PRIVATE ${THIRD_PARTY_FBX_SDK_LIBS})

add_dependencies(EngineCore EnginePlatform)
target_compile_definitions(EngineCore PUBLIC IMPORT_ENGINE_PLATFORM)
target_link_libraries(EngineCore PUBLIC EnginePlatform)


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

if(BUILD_STATIC)
    add_library(EngineRI STATIC ${ENGINE_RI_SRC})
else()
    add_library(EngineRI SHARED ${ENGINE_RI_SRC})
endif()

set_target_properties(EngineRI PROPERTIES OUTPUT_NAME "Engine_RI")
target_compile_definitions(EngineRI PRIVATE EXPORT_ENGINE_RI)

add_dependencies(EngineRI EnginePlatform EngineCore)
target_compile_definitions(EngineRI PUBLIC IMPORT_ENGINE_PLATFORM IMPORT_ENGINE_CORE)
target_link_libraries(EngineRI PUBLIC EnginePlatform EngineCore)

###############
# Editor Core #
###############

file(GLOB EDITOR_CORE_SRC
    "Editor/Core/*.cpp"
    "Editor/Core/*.h"
)

if(BUILD_STATIC)
    add_library(EditorCore STATIC ${EDITOR_CORE_SRC})
else()
    add_library(EditorCore SHARED ${EDITOR_CORE_SRC})
endif()

set_target_properties(EditorCore PROPERTIES OUTPUT_NAME "Editor_Core")
target_compile_definitions(EditorCore PRIVATE EXPORT_EDITOR_CORE)

add_dependencies(EditorCore EngineRI)
target_compile_definitions(EditorCore PUBLIC IMPORT_ENGINE_RI)
target_link_libraries(EditorCore PUBLIC EngineRI)


##########
# Engine #
##########
file(GLOB_RECURSE ENGINE_SRC
    "Engine/Engine/**.cpp"
    "Engine/Engine/**.h"
)

if(BUILD_STATIC)
    add_library(Engine STATIC ${ENGINE_SRC})
else()
    add_library(Engine SHARED ${ENGINE_SRC})
endif()

set_target_properties(Engine PROPERTIES OUTPUT_NAME "Engine")
target_compile_definitions(Engine PRIVATE EXPORT_ENGINE)
target_link_libraries(Engine PRIVATE ${THIRD_PARTY_PHYSX_SDK_LIBS} ${THIRD_PARTY_FBX_SDK_LIBS})

add_dependencies(Engine EngineCore EngineRI EditorCore)
target_compile_definitions(Engine PUBLIC IMPORT_ENGINE_CORE IMPORT_ENGINE_RI IMPORT_EDITOR_CORE)
target_link_libraries(Engine PUBLIC EngineCore EngineRI EditorCore)

##########
# Editor #
##########

file(GLOB EDITOR_SRC
    "Editor/Editor/*.cpp"
    "Editor/Editor/*.h"
    "Editor/Editor/Menu/*.cpp"
    "Editor/Editor/Menu/*.h"
    "Editor/Editor/Modal/*.cpp"
    "Editor/Editor/Modal/*.h"
    "Editor/Editor/Window/*.cpp"
    "Editor/Editor/Window/*.h"
)

if(BUILD_STATIC)
    add_library(Editor STATIC ${EDITOR_SRC})
else()
    add_library(Editor SHARED ${EDITOR_SRC})
endif()

set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor")
target_compile_definitions(Editor PRIVATE EXPORT_EDITOR)

add_dependencies(Editor Engine)
target_compile_definitions(Editor PUBLIC IMPORT_ENGINE)
target_link_libraries(Editor PUBLIC Engine)