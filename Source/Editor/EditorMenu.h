#pragma once

#include <string>

#include "Type.h"

namespace GameEngine
{
    class EditorMenu
    {
    public:
        EditorMenu (const std::string& title);
        virtual ~EditorMenu() = 0;

        void Render ();

    protected:
        bool MenuItem (const std::string& label);
        bool BeginSubMenu (const std::string& label);
        void EndSubMenu ();

        virtual void OnRender () = 0;

    private:
        std::string m_title;
    };
}
