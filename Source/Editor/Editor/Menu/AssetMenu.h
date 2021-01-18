#pragma once

#include "Type.h"
#include "Editor/Core/EditorMenu.h"

namespace GameEngine
{
    class AssetMenu : public EditorMenu
    {
    public:
        AssetMenu ();
        virtual ~AssetMenu();

    protected:
        virtual void OnRender () override;
    };
}
