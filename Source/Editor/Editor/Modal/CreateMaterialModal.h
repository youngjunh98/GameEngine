#pragma once

#include <string>

#include "Editor/Core/EditorModalWindow.h"

namespace GameEngine
{
    class CreateMaterialModal : public EditorModalWindow
    {
    public:
        CreateMaterialModal ();
        virtual ~CreateMaterialModal ();

        virtual void OnRender () override;

    private:
        std::string m_name;
    };
}
