#include "Editor.h"
#include "DearImGui.h"
#include "GameWindow.h"
#include "SceneWindow.h"
#include "InspectorWindow.h"
#include "ProjectWindow.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Engine/GameObject.h"
#include "Engine/Component/Component.h"
#include "Engine/Asset/AssetManager.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/Scene/SceneManager.h"

namespace GameEngine
{
	Editor::Editor () :
		m_editingGameObject (nullptr),
		m_bShowAssetEditWindow (false),
		m_bCreatingScene (false),
		m_bOpeningScene (false),
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
	}

	void Editor::Shutdown ()
	{
		m_windows.clear ();

		DearImGui::CleanUp ();
	}

	void Editor::RenderGUI ()
	{
		DearImGui::StartRender ();

		for (auto& window : m_windows)
		{
			window->Render ();
		}

		RenderMainMenuBar ();
		RenderAssetEditWindow ();

		RenderingInterface& renderingInterface = g_renderer.GetRenderingInterface ();

		for (uint32 i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT; i++)
		{
			renderingInterface.BindVertexShaderResource (nullptr, i);
			renderingInterface.BindPixelShaderResource (nullptr, i);
		}

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

	void Editor::BeginComponent (const std::string& name, Component* component, bool bDeletable)
	{
		AddLabel (name);
		
		if (bDeletable)
		{
			AddNextItemSameLine ();

			ImGui::PushID (name.c_str ());

			if (ImGui::Button ("Edit"))
			{
				ImGui::OpenPopup ("edit-popup");
			}

			if (ImGui::BeginPopup ("edit-popup"))
			{
				if (ImGui::Selectable ("Delete"))
				{
					component->Destroy ();
				}

				ImGui::EndPopup ();
			}

			ImGui::PopID ();
		}
	}

	void Editor::AddNextItemSameLine ()
	{
		ImGui::SameLine ();
	}

	void Editor::AddLabel (const std::string& name)
	{
		ImGui::Text (name.c_str ());
	}

	void Editor::AddCheckbox (const std::string& name, bool& bChecked)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		ImGui::Checkbox (("##" + name).c_str (), &bChecked);
	}

	void Editor::AddCombo (const std::string& name, int32& selectedIndex, const std::vector<std::string>& names)
	{
		if (names.size () <= 0)
		{
			return;
		}

		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		if (ImGui::BeginCombo (("##" + name).c_str (), names.at (selectedIndex).c_str ()))
		{
			for (size_t i = 0; i < names.size (); i++)
			{
				bool bSelected = i == static_cast<size_t> (selectedIndex);

				if (ImGui::Selectable (names.at (i).c_str (), &bSelected))
				{
					selectedIndex = static_cast<int32> (i);
				}

				if (bSelected)
				{
					ImGui::SetItemDefaultFocus ();
				}
			}

			ImGui::EndCombo ();
		}
	}

	void Editor::AddColorRGBA (const std::string& name, Vector4& rgba)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		ImGui::ColorEdit4 (("##" + name).c_str (), reinterpret_cast<float*> (&rgba), ImGuiColorEditFlags_Float);
	}

	void Editor::AddPropertyFloat (const std::string& name, float& value)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		ImGui::DragFloat (("##" + name).c_str (), &value, 0.1f);
	}

	void Editor::AddPropertyVector2 (const std::string& name, Vector2& value)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		ImGui::DragFloat2 (("##" + name).c_str (), reinterpret_cast<float*> (&value), 0.1f);
	}

	void Editor::AddPropertyVector3 (const std::string& name, Vector3& value)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		ImGui::DragFloat3 (("##" + name).c_str (), reinterpret_cast<float*> (&value), 0.1f);
	}

	void Editor::AddPropertyVector4 (const std::string& name, Vector4& value)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		ImGui::DragFloat4 (("##" + name).c_str (), reinterpret_cast<float*> (&value), 0.1f);
	}

	void Editor::AddPropertyAsset (const std::string& name, const std::string& type, std::wstring& path)
	{
		ImGui::Text (name.c_str ());
		ImGui::SameLine ();

		auto assetName = g_assetManager.GetAssetName (g_assetManager.FindAsset<Object> (path));
		std::string asciiAssetName (assetName.begin (), assetName.end ());

		std::string asciiPath (path.begin (), path.end ());
		ImGui::PushID ((name + asciiPath).c_str ());

		if (ImGui::Selectable (asciiAssetName.c_str ()))
		{
			ImGui::OpenPopup ("##select-asset");
		}

		if (ImGui::BeginPopup ("##select-asset"))
		{
			if (type == "Mesh")
			{
				for (auto* mesh : g_assetManager.FindAssets<Mesh> ())
				{
					assetName = g_assetManager.GetAssetName (mesh);
					asciiAssetName = std::string (assetName.begin (), assetName.end ());

					if (ImGui::Selectable (asciiAssetName.c_str ()))
					{
						path = g_assetManager.GetAssetPath (mesh);
					}
				}
			}
			else if (type == "Shader")
			{
				for (auto* shader : g_assetManager.FindAssets<Shader> ())
				{
					assetName = g_assetManager.GetAssetName (shader);
					asciiAssetName = std::string (assetName.begin (), assetName.end ());

					if (ImGui::Selectable (asciiAssetName.c_str ()))
					{
						path = g_assetManager.GetAssetPath (shader);
					}
				}
			}
			else if (type == "Material")
			{
				for (auto* material : g_assetManager.FindAssets<Material> ())
				{
					assetName = g_assetManager.GetAssetName (material);
					asciiAssetName = std::string (assetName.begin (), assetName.end ());

					if (ImGui::Selectable (asciiAssetName.c_str ()))
					{
						path = g_assetManager.GetAssetPath (material);
					}
				}
			}
			else if (type == "Texture")
			{
				for (auto* texture : g_assetManager.FindAssets<Texture> ())
				{
					assetName = g_assetManager.GetAssetName (texture);
					asciiAssetName = std::string (assetName.begin (), assetName.end ());

					if (ImGui::Selectable (asciiAssetName.c_str ()))
					{
						path = g_assetManager.GetAssetPath (texture);
					}
				}
			}

			ImGui::EndPopup ();
		}

		ImGui::PopID ();
	}

	void Editor::AddPropertyShaderParameter (const std::string& name, std::string& type, void* value)
	{
		ImGui::PushID (name.c_str ());

		if (ImGui::Selectable (type.c_str ()))
		{
			ImGui::OpenPopup ("##select-type");
		}

		if (type == "Float")
		{
			auto* floatValue = reinterpret_cast<float*> (value);
			AddPropertyFloat (name, *floatValue);
		}
		else if (type == "Vector")
		{
			auto* vector4Value = reinterpret_cast<Vector4*> (value);
			AddPropertyVector4 (name, *vector4Value);
		}

		if (ImGui::BeginPopup ("##select-type"))
		{
			if (ImGui::Selectable ("Float"))
			{
				type = "Float";
				ImGui::CloseCurrentPopup ();
			}
			
			if (ImGui::Selectable ("Vector"))
			{
				type = "Vector";
				ImGui::CloseCurrentPopup ();
			}

			ImGui::EndPopup ();
		}

		ImGui::PopID ();
	}

	void Editor::RenderMainMenuBar ()
	{
		if (ImGui::BeginMainMenuBar ())
		{
			RenderFileMenu ();

			if (ImGui::BeginMenu ("Edit"))
			{
				RenderEditMenu ();

				ImGui::EndMenu ();
			}

			if (ImGui::BeginMenu ("Asset"))
			{
				RenderAssetMenu ();

				ImGui::EndMenu ();
			}

			if (ImGui::BeginMenu ("Window"))
			{
				RenderWindowMenu ();

				ImGui::EndMenu ();
			}

			ImGui::EndMainMenuBar ();
		}

		if (m_bCreatingScene)
		{
			ImGui::SetNextWindowSize (ImVec2 (300, 100));
			ImGui::OpenPopup ("Create Scene");
			m_bCreatingScene = false;
		}
		else if (m_bOpeningScene)
		{
			ImGui::SetNextWindowSize (ImVec2 (300, 100));
			ImGui::OpenPopup ("Open Scene");
			m_bOpeningScene = false;
		}

		if (ImGui::BeginPopupModal ("Create Scene"))
		{
			ImGui::Text ("Scene Path");

			ImGui::Text ("Assets/");
			ImGui::SameLine ();

			ImGui::InputText ("##name-label", &m_editingScenePath, ImGuiInputTextFlags_None);

			if (ImGui::Button ("Create"))
			{
				CreateScene ("Assets/" + m_editingScenePath);
				ImGui::CloseCurrentPopup ();
			}

			ImGui::SameLine ();

			if (ImGui::Button ("Cancel"))
			{
				ImGui::CloseCurrentPopup ();
			}

			ImGui::EndPopup ();
		}

		if (ImGui::BeginPopupModal ("Open Scene"))
		{
			ImGui::Text ("Scene Path");

			ImGui::Text ("Assets/");
			ImGui::SameLine ();

			ImGui::InputText ("##name-label", &m_editingScenePath, ImGuiInputTextFlags_None);

			if (ImGui::Button ("Open"))
			{
				OpenScene ("Assets/" + m_editingScenePath);
				ImGui::CloseCurrentPopup ();
			}

			ImGui::SameLine ();

			if (ImGui::Button ("Cancel"))
			{
				ImGui::CloseCurrentPopup ();
			}

			ImGui::EndPopup ();
		}
	}

	void Editor::RenderAssetEditWindow ()
	{
		if (m_bShowAssetEditWindow == false)
		{
			return;
		}
		/*
		std::string asciiPath (m_selectedAssetPath.begin (), m_selectedAssetPath.end ());
		asciiPath = "##" + asciiPath;

		ImGui::PushID (asciiPath.c_str ());
		ImGui::Begin ("Edit Asset", &m_bShowAssetEditWindow, ImGuiWindowFlags_None);

		Object* asset = g_assetManager.FindAsset<Object> (m_selectedAssetPath);

		if (asset != nullptr)
		{
			asset->OnRenderEditor (*this);
		}

		if (ImGui::Button ("Save"))
		{
			g_assetManager.SaveAsset (m_selectedAssetPath);
		}

		ImGui::End ();
		ImGui::PopID ();
		*/
	}

	void Editor::RenderFileMenu ()
	{
		if (ImGui::BeginMenu ("File"))
		{
			if (ImGui::MenuItem ("Create Scene"))
			{
				m_bCreatingScene = true;
			}

			if (ImGui::MenuItem ("Open Scene"))
			{
				m_bOpeningScene = true;
			}

			if (ImGui::MenuItem ("Save Scene"))
			{
				SaveScene ();
			}

			ImGui::EndMenu ();
		}
	}

	void Editor::RenderEditMenu ()
	{
		if (ImGui::MenuItem ("Solid"))
		{
			g_renderer.BindRenderPipeline (g_renderer.GetDefaultForwardRenderPipeline ());
		}

		if (ImGui::MenuItem ("Wireframe"))
		{
			g_renderer.BindRenderPipeline (g_renderer.GetDefaultLineRenderPipeline ());
		}
	}

	void Editor::RenderAssetMenu ()
	{
		if (ImGui::MenuItem ("Import Asset"))
		{
			ImportAsset ();
		}
	}

	void Editor::RenderWindowMenu ()
	{
		if (ImGui::MenuItem ("Game", nullptr, nullptr))
		{
			OpenWindow<GameWindow> ();
		}

		if (ImGui::MenuItem ("Scene", nullptr, nullptr))
		{
			OpenWindow<SceneWindow> ();
		}

		if (ImGui::MenuItem ("Inspector", nullptr, nullptr))
		{
			OpenWindow<InspectorWindow> ();
		}

		if (ImGui::MenuItem ("Project", nullptr, nullptr))
		{
			OpenWindow<ProjectWindow> ();
		}
	}

	void Editor::CreateScene (const std::string& path)
	{
		SetEditingGameObject (nullptr);

		std::wstring unicodePath (path.begin (), path.end ());
		g_sceneManager.CreateEmptyScene (unicodePath);
	}

	void Editor::OpenScene (const std::string& path)
	{
		SetEditingGameObject (nullptr);

		std::wstring unicodePath (path.begin (), path.end ());
		g_sceneManager.LoadScene (unicodePath);
	}

	void Editor::SaveScene ()
	{
		g_sceneManager.SaveScene ();
	}

	void Editor::ImportAsset ()
	{
	}
}