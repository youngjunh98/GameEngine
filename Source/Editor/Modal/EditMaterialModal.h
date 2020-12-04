#pragma once

#include "Editor/EditorModalWindow.h"
#include "Engine/Rendering/Material.h"

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
