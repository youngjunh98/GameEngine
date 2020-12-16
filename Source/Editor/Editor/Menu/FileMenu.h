#pragma once

#include "Type.h"
#include "Editor/Core/EditorMenu.h"

namespace GameEngine
{
    class FileMenu : public EditorMenu
    {
    public:
        FileMenu ();
        virtual ~FileMenu();

    protected:
        virtual void OnRender () override;
    };
}
