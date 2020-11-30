#pragma once

#include <string>

namespace GameEngine
{
    class EditorWindow
    {
    public:
        EditorWindow (const std::string& title);
        virtual ~EditorWindow () = 0;

        void Render ();
        virtual void OnRender () = 0;

        void Show ();
        void Close ();

    private:
        bool m_bShow;
        std::string m_title;
    };
}
