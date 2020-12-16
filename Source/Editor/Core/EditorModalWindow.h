#pragma once

#include "EditorWindow.h"
#include "Editor/Core/EditorCoreMacro.h"

namespace GameEngine
{
    class EDITOR_CORE_API EditorModalWindow : public EditorWindow
    {
    public:
        EditorModalWindow (const std::string& title);
        virtual ~EditorModalWindow () = 0;

        virtual void Render () override;
        virtual void OnRender () = 0;
    };
}
