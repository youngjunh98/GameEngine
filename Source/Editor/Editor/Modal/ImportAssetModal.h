#pragma once

#include <string>

#include "Editor/Core/EditorModalWindow.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
    class Object;
    class Mesh;
    class Texture2D;
    class AudioClip;

    class ImportAssetModal : public EditorModalWindow
    {
    public:
        ImportAssetModal ();
        virtual ~ImportAssetModal ();

        virtual void OnRender () override;

    private:
        void RenderDirectory ();

        void RegisterAsset (const std::shared_ptr<Object>& asset, const PathString& path);
        bool ImportMeshAsset (const PathString& path, Mesh& mesh);
        bool ImportTexture2DAsset (const PathString& path, Texture2D& texture);
        bool ImportAudioAsset (const PathString& path, AudioClip& audioClip);

    private:
        PathString m_path;
    };
}
