#include "EditorWindow.h"
#include "DearImGui.h"

namespace GameEngine
{
    EditorWindow::EditorWindow (const std::string& title) : m_bShow (true), m_title (title)
    {
    }

    EditorWindow::~EditorWindow ()
    {
    }

    void EditorWindow::Render ()
    {
        if (m_bShow)
        {
            if (ImGui::Begin (m_title.c_str (), &m_bShow, ImGuiWindowFlags_NoCollapse))
            {
                OnRender ();
            }

            ImGui::End ();
        }
    }

    void EditorWindow::Show ()
    {
        m_bShow = true;
    }

    void EditorWindow::Close ()
    {
        m_bShow = false;
    }
}
