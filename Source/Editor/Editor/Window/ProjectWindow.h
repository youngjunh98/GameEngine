#pragma once

#include "Type.h"
#include "Editor/Core/EditorWindow.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
    class Object;

    class ProjectWindow final : public EditorWindow
    {
    public:
        ProjectWindow ();
        virtual ~ProjectWindow ();

        virtual void OnRender () override;

    private:
        void RenderDirectoryRecursive (PathString directoryName, PathString pathToDirectory);
        void RenderCreateMenu ();
        void RenderEditMenu ();

    private:
        PathString m_selectedAssetPath;
        Object* m_hoveredAsset;
    };
}
