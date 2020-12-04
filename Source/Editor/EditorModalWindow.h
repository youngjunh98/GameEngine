#pragma once

#include "EditorWindow.h"

namespace GameEngine
{
    class EditorModalWindow : public EditorWindow
    {
    public:
        EditorModalWindow (const std::string& title);
        virtual ~EditorModalWindow () = 0;

        virtual void Render () override;
        virtual void OnRender () = 0;
    };
}
