#include "Editor.h"
#include "Menu/FileMenu.h"
#include "Menu/WindowMenu.h"
#include "Menu/AssetMenu.h"
#include "Window/GameWindow.h"
#include "Window/InspectorWindow.h"
#include "Window/ProjectWindow.h"
#include "Window/SceneWindow.h"
#include "Window/ViewportWindow.h"
#include "Engine/RI/RenderingInterface.h"
#include "Engine/Engine/Engine.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Rendering/GlobalRenderer.h"
#include "Editor/Core/DearImGui.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	auto EditorRenderCallback = []() { Editor::GetInstance ().RenderGUI (); };

	Editor::Editor () :
		m_editingGameObject (nullptr),
		m_bShowAssetEditWindow (false),
		m_bCreatingAsset (false)
	{
	}

	Editor::~Editor ()
	{
	}

	Editor& Editor::GetInstance ()
	{
		static Editor editor;
		return editor;
	}

	bool Editor::Initialize ()
	{
		if (Engine::GetInstance ().Init () == false)
		{
			return false;
		}

		Engine::GetInstance ().AddPostRenderCallback (EditorRenderCallback);

		if (DearImGui::Initialize () == false)
		{
			return false;
		}

		m_menus.push_back (std::make_unique<FileMenu> ());
		m_menus.push_back (std::make_unique<WindowMenu> ());
		m_menus.push_back (std::make_unique<AssetMenu> ());

		OpenWindow<GameWindow> ();
		OpenWindow<InspectorWindow> ();
		OpenWindow<ProjectWindow> ();
		OpenWindow<SceneWindow> ();
		OpenWindow<ViewportWindow> ();

		return true;
	}

	void Editor::Shutdown ()
	{
		Engine::GetInstance ().RemovePostRenderCallback (EditorRenderCallback);

		m_modalWindow = nullptr;
		m_menus.clear ();
		m_windows.clear ();

		DearImGui::CleanUp ();
		Engine::GetInstance ().Shutdown ();
	}

	void Editor::Run ()
	{
		Engine::GetInstance ().Run ();
	}

	void Editor::RenderGUI ()
	{
		DearImGui::StartRender ();

		if (DearImGui::BeginMainMenuBar ())
		{
			for (auto& menu : m_menus)
			{
				menu->Render ();
			}

			DearImGui::EndMainMenuBar ();
		}
		
		for (auto& window : m_windows)
		{
			window->Render ();
		}

		if (m_modalWindow != nullptr)
		{
			m_modalWindow->Render ();

			if (m_modalWindow->IsOpen () == false)
			{
				m_modalWindow = nullptr;
			}
		}

		RenderingInterface& renderingInterface = RenderingInterface::GetModule ();
		RI_RenderTargetView* swapChainRenderTarget = GlobalRenderer::GetSwapChainRenderTarget ();
		RI_DepthStencilView* screenDepthStencil = GlobalRenderer::GetScreenDepthStencil ();
		
		renderingInterface.BindRenderTargetViewAndDepthStencilView (swapChainRenderTarget, screenDepthStencil);
		renderingInterface.ClearRenderTarget (swapChainRenderTarget, 0.0f, 0.0f, 0.0f, 1.0f);
		renderingInterface.ClearDepthStencil (screenDepthStencil, 1.0f, 0);

		DearImGui::FinishRender ();
	}

	GameObject* Editor::GetEditingGameObject () const
	{
		return m_editingGameObject;
	}

	void Editor::SetEditingGameObject (GameObject* gameObject)
	{
		m_editingGameObject = gameObject;
	}

	EditorModalWindow* Editor::GetModalWindow () const
	{
		return m_modalWindow.get ();
	}
}