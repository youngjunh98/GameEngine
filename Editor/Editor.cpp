#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"

#include "Editor/Editor.h"
#include "GameObject.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/Physics/Rigidbody.h"
#include "Component/Physics/BoxCollider.h"
#include "Component/Physics/SphereCollider.h"
#include "Component/Rendering/MeshRenderer.h"
#include "Component/Rendering/Light.h"
#include "Platform/Platform.h"
#include "Asset/AssetManager.h"
#include "Rendering/GlobalRenderer.h"
#include "Scene/SceneManager.h"

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler (HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GameEngine
{
	Editor::Editor () :
		m_bShowGameWindow (true),
		m_bShowHierarchyWindow (true),
		m_bShowInspectorWindow (true),
		m_bShowAssetBrowserWindow (true),
		m_bShowAssetEditWindow (false),
		m_bCreatingScene (false),
		m_bOpeningScene (false),
		m_bCreatingAsset (false),
		m_selectedGameObject (nullptr),
		m_selectedGameObjectIndex (0)
	{
	}


	Editor::~Editor ()
	{
	}

	void Editor::Initialize ()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION ();
		ImGui::CreateContext ();
		ImGuiIO& io = ImGui::GetIO (); (void) io;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark ();

		auto& style = ImGui::GetStyle ();
		style.WindowRounding = 1.0f;

		ImVec4 clear_color = ImVec4 (0.45f, 0.55f, 0.60f, 1.00f);

		// Setup Platform/Renderer bindings
		PlatformRenderingInterface& renderingInterface = g_renderer.GetPlatformRenderingInterface ();
		auto* d3d11Device = reinterpret_cast<ID3D11Device*> (renderingInterface.GetNativeDevice ());
		auto* d3d11Context = reinterpret_cast<ID3D11DeviceContext*> (renderingInterface.GetNativeContext ());

		ImGui_ImplWin32_Init (Platform::GetGenericApplication ().GetNativeWindowHandle ());
		ImGui_ImplDX11_Init (d3d11Device, d3d11Context);

		// Init Editor
		auto& winApp = static_cast<PlatformApplication&> (Platform::GetGenericApplication ());
		winApp.AddWindowProcedureCallback (ImGui_ImplWin32_WndProcHandler);

		m_gameRenderBuffer = renderingInterface.CreateTexture2D (800, 800, 1, 1, ERenderingResourceFormat::R8G8B8A8_UNorm, nullptr, nullptr, false, true, true, false);
		m_gameRenderTarget = renderingInterface.CreateRenderTargetView (m_gameRenderBuffer.get (), 0, 1, 0);
		m_gameRenderTexture = renderingInterface.CreateShaderResourceView (m_gameRenderBuffer.get(), ERenderingResourceFormat::R8G8B8A8_UNorm, 0, 1, 0);

		m_gameDepthStencilBuffer = renderingInterface.CreateTexture2D (800, 800, 1, 1, ERenderingResourceFormat::R24G8_Typeless, nullptr, nullptr, false, true, false, true);
		m_gameDepthStencil = renderingInterface.CreateDepthStencilView (m_gameDepthStencilBuffer.get (), 0, 1, 0);
		m_gameDepthStencilTexture = renderingInterface.CreateShaderResourceView (m_gameDepthStencilBuffer.get (), ERenderingResourceFormat::R24_UNorm_X8_Typeless, 0, 1, 0);

		g_renderer.SetRenderTarget (m_gameRenderTarget, m_gameDepthStencil);
	}

	void Editor::Shutdown ()
	{
		m_gameRenderBuffer = nullptr;
		m_gameRenderTarget = nullptr;
		m_gameRenderTexture = nullptr;

		m_gameDepthStencilBuffer = nullptr;
		m_gameDepthStencil = nullptr;
		m_gameDepthStencilTexture = nullptr;

		// Cleanup
		ImGui_ImplDX11_Shutdown ();
		ImGui_ImplWin32_Shutdown ();
		ImGui::DestroyContext ();
	}

	void Editor::RenderGUI ()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame ();
		ImGui_ImplWin32_NewFrame ();
		ImGui::NewFrame ();

		// Render Editor GUI
		PlatformRenderingInterface& renderingInterface = g_renderer.GetPlatformRenderingInterface ();

		RenderMainMenuBar ();
		RenderGameWindow ();
		RenderHierarchyWindow ();
		RenderInspectorWindow ();
		RenderAssetBrowserWindow ();
		RenderAssetEditWindow ();

		for (uint32 i = 0; i < D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT; i++)
		{
			renderingInterface.BindVertexShaderResource (nullptr, i);
			renderingInterface.BindPixelShaderResource (nullptr, i);
		}

		RI_RenderTargetView* screenRenderTarget = g_renderer.GetScreenRenderTarget ();
		RI_DepthStencilView* screenDepthStencil = g_renderer.GetScreenDepthStencil ();
		
		renderingInterface.BindRenderTargetViewAndDepthStencilView (screenRenderTarget, screenDepthStencil);
		renderingInterface.ClearRenderTarget (screenRenderTarget, 0.0f, 0.0f, 0.0f, 1.0f);
		renderingInterface.ClearDepthStencil (screenDepthStencil, 1.0f, 0);

		// Rendering
		ImGui::Render ();
		ImGui_ImplDX11_RenderDrawData (ImGui::GetDrawData ());
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

			ImGui::InputText ("##name-label", &m_scenePath, ImGuiInputTextFlags_None);

			if (ImGui::Button ("Create"))
			{
				CreateScene ("Assets/" + m_scenePath);
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

			ImGui::InputText ("##name-label", &m_scenePath, ImGuiInputTextFlags_None);

			if (ImGui::Button ("Open"))
			{
				OpenScene ("Assets/" + m_scenePath);
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

	void Editor::RenderGameWindow ()
	{
		if (m_bShowGameWindow == false)
		{
			return;
		}

		ImGui::Begin ("Game", &m_bShowGameWindow, ImGuiWindowFlags_None);

		auto contentMin = ImGui::GetWindowContentRegionMin ();
		auto contentMax = ImGui::GetWindowContentRegionMax ();

		auto* image = reinterpret_cast<D3D11RI_ShaderResourceView*> (m_gameRenderTexture.get ())->m_resource.Get ();

		ImGui::Image (image, ImVec2 (contentMax.x - contentMin.x, contentMax.y - contentMin.y));

		ImGui::End ();
	}

	void Editor::RenderHierarchyWindow ()
	{
		if (m_bShowHierarchyWindow == false)
		{
			return;
		}

		ImGui::Begin ("Hierarchy", &m_bShowHierarchyWindow, ImGuiWindowFlags_None);

		auto* scene = g_sceneManager.GetScene ();

		if (scene != nullptr)
		{
			if (ImGui::BeginPopupContextWindow ("edit-gameobject-popup"))
			{
				if (ImGui::Selectable ("Add GameObject"))
				{
					scene->SpawnGameObject ();
				}

				if (ImGui::Selectable ("Delete GameObject"))
				{
					if (m_selectedGameObject != nullptr)
					{
						m_selectedGameObject->Destroy ();
						m_selectedGameObject = nullptr;
					}
				}

				ImGui::EndPopup ();
			}

			if (ImGui::TreeNode (m_scenePath.c_str ()))
			{
				auto& gameObjects = scene->GetGameObjects ();

				for (size_t i = 0; i < gameObjects.size (); i++)
				{
					if (gameObjects[i]->IsAlive () == false)
					{
						continue;
					}

					bool bSelected = gameObjects[i].get () == m_selectedGameObject;
					std::string label = gameObjects[i]->GetName () + "##" + std::to_string (i);

					if (ImGui::Selectable (label.c_str (), &bSelected, ImGuiSelectableFlags_None))
					{
						m_selectedGameObject = gameObjects[i].get ();
						m_selectedGameObjectIndex = i;

						break;
					}
				}

				ImGui::TreePop ();
			}
		}

		ImGui::End ();
	}

	void Editor::RenderInspectorWindow ()
	{
		if (m_bShowInspectorWindow == false)
		{
			return;
		}

		ImGui::Begin ("Inspector", &m_bShowInspectorWindow, ImGuiWindowFlags_None);

		if (m_selectedGameObject != nullptr)
		{
			ImGui::PushID (m_selectedGameObjectIndex);
			bool bActive = m_selectedGameObject->IsActive ();

			ImGui::Checkbox ("##active-checkbox", &bActive);
			ImGui::SameLine ();

			m_selectedGameObject->SetActive (bActive);

			std::string name = m_selectedGameObject->GetName ();
			std::string tag = m_selectedGameObject->GetTag ();

			ImGui::Text ("Name");
			ImGui::SameLine ();

			ImGui::InputText ("##name-label", &name, ImGuiInputTextFlags_None);

			ImGui::Text ("Tag");
			ImGui::SameLine ();

			ImGui::InputText ("##tag-label", &tag, ImGuiInputTextFlags_None);

			m_selectedGameObject->SetName (name);
			m_selectedGameObject->SetTag (tag);

			int32 id = 0;

			for (auto* component : m_selectedGameObject->GetComponents<Component> ())
			{
				if (component->IsAlive () == false)
				{
					continue;
				}

				ImGui::Separator ();
				ImGui::PushID (id);

				component->OnRenderEditor (*this);

				ImGui::PopID ();
				id += 1;
			}

			ImGui::Separator ();
			ImGui::Spacing ();

			if (ImGui::Button ("Add Component"))
			{
				ImGui::OpenPopup ("##add-component-popup");
			}

			if (ImGui::BeginPopup ("##add-component-popup"))
			{
				if (ImGui::Selectable ("Camera"))
				{
					m_selectedGameObject->AddComponent<Camera> ();
				}

				if (ImGui::Selectable ("Mesh Renderer"))
				{
					m_selectedGameObject->AddComponent<MeshRenderer> ();
				}

				if (ImGui::Selectable ("Light"))
				{
					m_selectedGameObject->AddComponent<Light> ();
				}

				if (ImGui::Selectable ("Rigidbody"))
				{
					m_selectedGameObject->AddComponent<Rigidbody> ();
				}

				if (ImGui::Selectable ("Box Collider"))
				{
					m_selectedGameObject->AddComponent<BoxCollider> ();
				}

				if (ImGui::Selectable ("Sphere Collider"))
				{
					m_selectedGameObject->AddComponent<SphereCollider> ();
				}

				ImGui::EndPopup ();
			}

			ImGui::PopID ();
		}

		ImGui::End ();
	}

	void Editor::RenderAssetBrowserWindow ()
	{
		if (m_bShowAssetBrowserWindow == false)
		{
			return;
		}

		ImGui::Begin ("Asset Browser", &m_bShowAssetBrowserWindow, ImGuiWindowFlags_None);

		RenderAssetTree (L"Assets", L"");

		if (ImGui::BeginPopupContextWindow ("##create-asset"))
		{
			if (ImGui::Selectable ("Create Material"))
			{
				m_bCreatingAsset = true;
			}

			ImGui::EndPopup ();
		}

		if (m_bCreatingAsset)
		{
			m_bCreatingAsset = false;
			ImGui::OpenPopup ("##create-asset-popup");
		}

		if (ImGui::BeginPopupModal ("##create-asset-popup"))
		{
			ImGui::Text ("Path");

			ImGui::Text ("Assets/");
			ImGui::SameLine ();

			ImGui::InputText ("##create-asset-path", &m_createAssetPath, ImGuiInputTextFlags_None);
			std::string path = "Assets/" + m_createAssetPath;
			std::wstring unicodePath (path.begin (), path.end ());

			if (ImGui::Button ("Create"))
			{
				PlatformFile file;
				file.Open (unicodePath, false, true);

				if (file.IsOpen ())
				{
					auto material = std::make_shared<Material> ();
					material->SetShader (g_assetManager.LoadShader (L"Assets/Shader/StandardShader.hlsl"));

					std::string jsonString = Json::JsonSerializer::Serialize (*material).dump ();
					int64 jsonStringSize = jsonString.size ();

					file.Write (jsonString.data (), jsonStringSize);

					g_assetManager.AddAsset (material, unicodePath);

					m_selectedAssetPath = unicodePath;
					m_bShowAssetEditWindow = true;
				}

				ImGui::CloseCurrentPopup ();
			}

			ImGui::SameLine ();

			if (ImGui::Button ("Cancel"))
			{
				ImGui::CloseCurrentPopup ();
			}

			ImGui::EndPopup ();
		}

		ImGui::End ();
	}

	void Editor::RenderAssetTree (const std::wstring& directory, const std::wstring& path)
	{
		auto& fileSystem = Platform::GetGenericFileSystem ();
		auto searchPath = path + directory + L"/";

		std::string asciiDirectory (directory.begin (), directory.end ());

		if (ImGui::TreeNode (asciiDirectory.c_str ()))
		{
			for (auto& directory : fileSystem.GetDirectoryList (searchPath))
			{
				RenderAssetTree (directory, searchPath);
			}

			for (auto& file : fileSystem.GetFileList (searchPath))
			{
				auto filePath = searchPath + file;
				bool bSelected = filePath == m_selectedAssetPath;

				std::string asciiFile (file.begin (), file.end ());

				if (ImGui::Selectable (asciiFile.c_str (), &bSelected))
				{
					m_selectedAssetPath = filePath;
					m_bShowAssetEditWindow = true;
				}
			}

			ImGui::TreePop ();
		}
	}

	void Editor::RenderAssetEditWindow ()
	{
		if (m_bShowAssetEditWindow == false)
		{
			return;
		}

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
		ImGui::MenuItem ("Game", nullptr, &m_bShowGameWindow);
		ImGui::MenuItem ("Hierarchy", nullptr, &m_bShowHierarchyWindow);
		ImGui::MenuItem ("Inspector", nullptr, &m_bShowInspectorWindow);
		ImGui::MenuItem ("Asset Browser", nullptr, &m_bShowAssetBrowserWindow);
	}

	void Editor::CreateScene (const std::string& path)
	{
		m_selectedGameObject = nullptr;
		std::wstring unicodePath (path.begin (), path.end ());

		g_sceneManager.CreateEmptyScene (unicodePath);
	}

	void Editor::OpenScene (const std::string& path)
	{
		m_selectedGameObject = nullptr;
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