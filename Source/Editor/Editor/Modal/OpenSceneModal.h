#pragma once

#include <string>

#include "Editor/Core/EditorModalWindow.h"

namespace GameEngine
{
    class OpenSceneModal : public EditorModalWindow
    {
    public:
        OpenSceneModal ();
        virtual ~OpenSceneModal ();

        virtual void OnRender () override;

    private:
        std::string m_scenePath;
    };
}
