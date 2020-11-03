project(Editor)


#########
# ImGUI #
#########

file(GLOB IMGUI_SRC
    "ImGui/*.cpp"
    "ImGui/*.h"
    "ImGui/examples/*.cpp"
    "ImGui/examples/*.h"
    "ImGui/misc/*.cpp"
    "ImGui/misc/*.h"
    "ImGui/misc/cpp/*.cpp"
    "ImGui/misc/cpp/*.h"
)


##########
# Editor #
##########

file(GLOB EDITOR_SRC
    "*.cpp"
    "*.h"
)

#[[
if(BUILD_STATIC)
    add_library(Editor STATIC ${EDITOR_SRC} ${IMGUI_SRC})
else()
    add_library(Editor SHARED ${EDITOR_SRC} ${IMGUI_SRC})
endif()

target_compile_definitions(Editor PRIVATE
    EXPORT_EDITOR
    IMPORT_CORE
    IMPORT_RI
)

target_link_libraries(Editor   
    Core
    RI
    Engine
)

add_dependencies(Editor
    Core
    RI
)

set_target_properties(Editor PROPERTIES OUTPUT_NAME "Editor_Editor")
]]