#pragma once

#include "Editor/Core/EditorModalWindow.h"
#include "Engine/Engine/Rendering/Material.h"

namespace GameEngine
{
    class EditMaterialModal : public EditorModalWindow
    {
    public:
        EditMaterialModal ();
        virtual ~EditMaterialModal ();

        virtual void OnRender () override;

        Material* GetMaterial () const;
        void SetMaterial (Material* material);

    private:
        Material* m_material;
    };
}
