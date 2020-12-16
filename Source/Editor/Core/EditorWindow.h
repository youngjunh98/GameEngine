#pragma once

#include <string>

#include "Editor/Core/EditorCoreMacro.h"

namespace GameEngine
{
    class EDITOR_CORE_API EditorWindow
    {
    public:
        EditorWindow (const std::string& title);
        virtual ~EditorWindow () = 0;

        virtual void Render ();

        void Show ();
        void Close ();
        bool IsOpen () const;

    protected:
        virtual void OnRender () = 0;

    protected:
        bool m_bShow;
        std::string m_title;
    };
}
