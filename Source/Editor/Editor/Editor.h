#pragma once

#include <vector>
#include <memory>
#include <utility>

#include "Type.h"
#include "Engine/Core/CoreMinimal.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/Engine/GameObject.h"
#include "Editor/Core/EditorMenu.h"
#include "Editor/Core/EditorWindow.h"
#include "Editor/Core/EditorModalWindow.h"
#include "Editor/Editor/EditorMacro.h"

namespace GameEngine
{
	class EDITOR_API Editor final
	{
	private:
		Editor ();
		virtual ~Editor ();

	public:
		Editor (const Editor&) = delete;
		Editor& operator= (const Editor&) = delete;

		static Editor& GetInstance ();

		bool Initialize ();
		void Shutdown ();

		void Run ();
		void RenderGUI ();

		GameObject* GetEditingGameObject () const;
		void SetEditingGameObject (GameObject* gameObject);

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

		template<typename TEditorModalWindow>
		TEditorModalWindow* OpenModalWindow ()
		{
			TEditorModalWindow* temp = nullptr;
			auto modalWindow = std::make_unique<TEditorModalWindow> ();

			if (modalWindow != nullptr)
			{
				temp = modalWindow.get ();

				m_modalWindow = std::move (modalWindow);
				m_modalWindow->Show ();
			}

			return temp;
		}

		EditorModalWindow* GetModalWindow () const;
		
	private:
		std::vector<std::unique_ptr<EditorMenu>> m_menus;
		std::vector<std::unique_ptr<EditorWindow>> m_windows;
		std::unique_ptr<EditorModalWindow> m_modalWindow;

		GameObject* m_editingGameObject;

		bool m_bShowAssetEditWindow;
		bool m_bCreatingAsset;

		std::string m_createAssetPath;
	};
}
