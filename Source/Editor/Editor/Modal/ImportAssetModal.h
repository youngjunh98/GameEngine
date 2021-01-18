#pragma once

#include <string>

#include "Editor/Core/EditorModalWindow.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
    class ImportAssetModal : public EditorModalWindow
    {
    public:
        ImportAssetModal ();
        virtual ~ImportAssetModal ();

        virtual void OnRender () override;

    private:
        void RenderDirectory ();

    private:
        PathString m_path;
    };
}
