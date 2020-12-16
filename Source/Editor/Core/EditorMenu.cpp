#include "EditorMenu.h"
#include "Editor/Core/DearImGui.h"

namespace GameEngine
{
    EditorMenu::EditorMenu (const std::string& title) : m_title (title)
    {
    }

    EditorMenu::~EditorMenu ()
    {
    }

    void EditorMenu::Render ()
    {
        if (ImGui::BeginMenu (m_title.c_str ()))
        {
            OnRender ();
            ImGui::EndMenu ();
        }
    }

    bool EditorMenu::MenuItem (const std::string& label)
    {
        return ImGui::MenuItem (label.c_str ());
    }

    bool EditorMenu::BeginSubMenu (const std::string& label)
    {
        return ImGui::BeginMenu (label.c_str ());
    }

    void EditorMenu::EndSubMenu ()
    {
        ImGui::EndMenu ();
    }
}
