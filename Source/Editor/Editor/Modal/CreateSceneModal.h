#pragma once

#include <string>

#include "Editor/Core/EditorModalWindow.h"

namespace GameEngine
{
    class CreateSceneModal : public EditorModalWindow
    {
    public:
        CreateSceneModal ();
        virtual ~CreateSceneModal ();

        virtual void OnRender () override;

    private:
        std::string m_scenePath;
    };
}
