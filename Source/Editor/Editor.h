#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "Type.h"
#include "EditorWindow.h"
#include "Core/CoreMinimal.h"
#include "Core/File/FileSystem.h"

namespace GameEngine
{
	class Editor final
	{
	private:
		Editor ();
		virtual ~Editor ();

	public:
		static Editor& GetInstance ();

		void Initialize ();
		void Shutdown ();

		void RenderGUI ();

		GameObject* GetEditingGameObject () const;
		void SetEditingGameObject (GameObject* gameObject);

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

		template<typename TEditorWindow>
		TEditorWindow* CreateWindowInstance ()
		{
			TEditorWindow* temp = nullptr;
			auto window = std::make_unique<TEditorWindow> ();

			if (window != nullptr)
			{
				temp = window.get ();
				m_windows.push_back (std::move (window));
			}

			return temp;
		}

		template<typename TEditorWindow>
		TEditorWindow* FindWindowInstance ()
		{
			TEditorWindow* found = nullptr;

			for (auto& window : m_windows)
			{
				auto* casted = dynamic_cast<TEditorWindow*> (window.get ());

				if (casted != nullptr)
				{
					found = casted;
					break;
				}
			}

			return found;
		}

		template<typename TEditorWindow>
		void OpenWindow ()
		{
			auto* window = FindWindowInstance<TEditorWindow> ();

			if (window == nullptr)
			{
				window = CreateWindowInstance<TEditorWindow> ();
			}

			window->Show ();
		}
	
	private:
		void RenderMainMenuBar ();
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
		std::vector<std::unique_ptr<EditorWindow>> m_windows;

		GameObject* m_editingGameObject;
        std::string m_editingScenePath;

		bool m_bShowAssetEditWindow;

		bool m_bCreatingScene;
		bool m_bOpeningScene;
		bool m_bCreatingAsset;

		std::string m_createAssetPath;
	};
}
