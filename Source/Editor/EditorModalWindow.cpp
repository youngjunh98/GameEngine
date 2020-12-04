#include "EditorModalWindow.h"
#include "Core/DearImGui.h"

namespace GameEngine
{
    EditorModalWindow::EditorModalWindow (const std::string& title) : EditorWindow (title)
    {
    }

    EditorModalWindow::~EditorModalWindow ()
    {
    }

    void EditorModalWindow::Render ()
    {
        if (m_bShow)
        {
            ImGui::OpenPopup (m_title.c_str ());
        }

        if (ImGui::BeginPopupModal (m_title.c_str (), &m_bShow, ImGuiWindowFlags_AlwaysAutoResize))
        {
            OnRender ();
            ImGui::EndPopup ();
        }
    }
}
