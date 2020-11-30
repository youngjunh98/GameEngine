#pragma once

#include "Type.h"
#include "EditorWindow.h"

namespace GameEngine
{
    class SceneWindow final : public EditorWindow
    {
    public:
        SceneWindow ();
        virtual ~SceneWindow ();

        virtual void OnRender () override;
    };
}
