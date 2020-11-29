#pragma once

#include "Core/CoreMinimal.h"
#include "Platform/PlatformRenderingInterface.h"

namespace GameEngine
{
	class Editor
	{
	public:
		Editor ();
		virtual ~Editor ();

		void Initialize ();
		void Shutdown ();

		void RenderGUI ();

		void BeginComponent (const std::string& name, Component* component, bool bDeletable);
		void AddNextItemSameLine ();

		void AddLabel (const std::string& name);
		void AddCheckbox (const std::string& name, bool& bChecked);
		void AddCombo (const std::string& name, int32& selectedIndex, const std::vector<std::string>& names);
		void AddColorRGBA (const std::string& name, Vector4& rgba);
		void AddPropertyFloat (const std::string& name, float& value);
		void AddPropertyVector2 (const std::string& name, Vector2& value);
		void AddPropertyVector3 (const std::string& name, Vector3& value);
		void AddPropertyVector4 (const std::string& name, Vector4& value);
		void AddPropertyAsset (const std::string& name, const std::string& type, std::wstring& path);
		void AddPropertyShaderParameter (const std::string& name, std::string& type, void* value);

	private:
		void RenderMainMenuBar ();
		void RenderGameWindow ();
		void RenderHierarchyWindow ();
		void RenderInspectorWindow ();
		void RenderAssetBrowserWindow ();
		void RenderAssetTree (const std::wstring& directory, const std::wstring& path);
		void RenderAssetEditWindow ();

		void RenderFileMenu ();
		void RenderEditMenu ();
		void RenderAssetMenu ();
		void RenderWindowMenu ();

		void CreateScene (const std::string& path);
		void OpenScene (const std::string& path);
		void SaveScene ();

		void ImportAsset ();

	private:
		bool m_bShowGameWindow;
		bool m_bShowHierarchyWindow;
		bool m_bShowInspectorWindow;
		bool m_bShowAssetBrowserWindow;
		bool m_bShowAssetEditWindow;

		bool m_bCreatingScene;
		bool m_bOpeningScene;
		bool m_bCreatingAsset;

		GameObject* m_selectedGameObject;
		int32 m_selectedGameObjectIndex;
		std::wstring m_selectedAssetPath;
		std::string m_createAssetPath;
		std::string m_scenePath;

		Vector2 m_gameViewSize;
		RenderingResourcePtr<RI_Texture2D> m_gameRenderBuffer;
		RenderingResourcePtr<RI_Texture2D> m_gameDepthStencilBuffer;
		RenderingResourcePtr<RI_RenderTargetView> m_gameRenderTarget;
		RenderingResourcePtr<RI_DepthStencilView> m_gameDepthStencil;
		RenderingResourcePtr<RI_ShaderResourceView> m_gameRenderTexture;
		RenderingResourcePtr<RI_ShaderResourceView> m_gameDepthStencilTexture;
	};
}
