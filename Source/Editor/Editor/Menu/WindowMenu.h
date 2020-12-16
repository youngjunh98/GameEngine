#pragma once

#include "Type.h"
#include "Editor/Core/EditorMenu.h"

namespace GameEngine
{
    class WindowMenu : public EditorMenu
    {
    public:
        WindowMenu ();
        virtual ~WindowMenu();

    protected:
        virtual void OnRender () override;
    };
}
