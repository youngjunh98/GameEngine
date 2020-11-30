#pragma once

#include "Type.h"
#include "EditorWindow.h"
#include "Core/File/FileSystem.h"

namespace GameEngine
{
    class ProjectWindow final : public EditorWindow
    {
    public:
        ProjectWindow ();
        virtual ~ProjectWindow ();

        virtual void OnRender () override;

    private:
        void RenderDirectoryTreeRecursive (PathString directoryName, PathString pathToDirectory);

    private:
        PathString m_selectedAssetPath;
    };
}
