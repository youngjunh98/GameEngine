#include "Editor.h"
#include "EditorGUI.h"
#include "Core/DearImGui.h"
#include "Menu/FileMenu.h"
#include "Menu/WindowMenu.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Engine/Rendering/GlobalRenderer.h"

namespace GameEngine
{
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

	void Editor::Initialize ()
	{
		if (DearImGui::Initialize () == false)
		{
			return;
		}

		m_menus.push_back (std::make_unique<FileMenu> ());
		m_menus.push_back (std::make_unique<WindowMenu> ());
	}

	void Editor::Shutdown ()
	{
		m_modalWindow = nullptr;
		m_menus.clear ();
		m_windows.clear ();

		DearImGui::CleanUp ();
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

		RenderingInterface& renderingInterface = g_renderer.GetRenderingInterface ();
		RI_RenderTargetView* swapChainRenderTarget = g_renderer.GetSwapChainRenderTarget ();
		RI_DepthStencilView* screenDepthStencil = g_renderer.GetScreenDepthStencil ();
		
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