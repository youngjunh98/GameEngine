#pragma once

#include "Type.h"
#include "Editor/EditorWindow.h"

namespace GameEngine
{
    class GameObject;

    class SceneWindow final : public EditorWindow
    {
    public:
        SceneWindow ();
        virtual ~SceneWindow ();

        virtual void OnRender () override;

    private:
        GameObject* m_hoveredGameObject;
    };
}
