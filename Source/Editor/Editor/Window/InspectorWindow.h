#pragma once

#include "Type.h"
#include "Editor/Core/EditorWindow.h"

namespace GameEngine
{
    class InspectorWindow final : public EditorWindow
    {
    public:
        InspectorWindow ();
        virtual ~InspectorWindow ();

        virtual void OnRender () override;
    };
}
