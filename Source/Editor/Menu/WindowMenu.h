#pragma once

#include "Type.h"
#include "Editor/EditorMenu.h"

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
