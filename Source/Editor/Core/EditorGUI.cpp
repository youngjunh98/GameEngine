#include "EditorGUI.h"
#include "Editor/Core/DearImGui.h"
#include "Engine/Core/File/FileSystem.h"
#include "Engine/Core/Asset/AssetManager.h"

namespace GameEngine
{
	EditorGUI::EditorGUI () {}
	EditorGUI::~EditorGUI () {}

	void EditorGUI::PushID (int32 id)
	{
		ImGui::PushID (id);
	}

	void EditorGUI::PushID (const std::string& id)
	{
		ImGui::PushID (id.c_str ());
	}

	void EditorGUI::PopID ()
	{
		ImGui::PopID ();
	}

	bool EditorGUI::IsWindowFocused ()
	{
		return ImGui::IsWindowFocused ();
	}

	bool EditorGUI::IsWindowHovered ()
	{
		return ImGui::IsWindowHovered ();
	}
	
	bool EditorGUI::IsLastItemClicked ()
	{
		return ImGui::IsItemClicked ();
	}

	bool EditorGUI::IsLastItemFocused ()
	{
		return ImGui::IsItemFocused ();
	}

	bool EditorGUI::IsLastItemHovered ()
	{
		return ImGui::IsItemHovered ();
	}

	Vector2 EditorGUI::GetWindowSize ()
	{
		ImVec2 windowSize = ImGui::GetWindowSize ();

		if (windowSize.x < 0.0f)
		{
			windowSize.x = 0.0f;
		}

		if (windowSize.y < 0.0f)
		{
			windowSize.y = 0.0f;
		}

		return Vector2 (windowSize.x, windowSize.y);
	}

	Vector2 EditorGUI::GetWindowContentRegionSize ()
	{
		ImVec2 contentMin = ImGui::GetWindowContentRegionMin ();
		ImVec2 contentMax = ImGui::GetWindowContentRegionMax ();

		float width = contentMax.x - contentMin.x;
		float height = contentMax.y - contentMin.y;

		if (width < 0.0f)
		{
			width = 0.0f;
		}

		if (height < 0.0f)
		{
			height = 0.0f;
		}

		return Vector2 (width, height);
	}

	Vector2 EditorGUI::GetWindowAvailableContentRegionSize ()
	{
		ImVec2 available = ImGui::GetContentRegionAvail ();
		float width = available.x < 0.0f ? 0.0f : available.x;
		float height = available.y < 0.0f ? 0.0f : available.y;

		return Vector2 (width, height);
	}

	void EditorGUI::SameLine ()
	{
		ImGui::SameLine ();
	}

	void EditorGUI::Separator ()
	{
		ImGui::Separator ();
	}

	void EditorGUI::Label (const std::string& label)
	{
		ImGui::Text (label.c_str ());
	}

	bool EditorGUI::SelectableLabel (const std::string& label, bool bSelected, bool bDisabled)
	{
		ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;

		if (bDisabled)
		{
			flags |= ImGuiSelectableFlags_Disabled;
		}

		return ImGui::Selectable (label.c_str (), bSelected, flags);
	}

	void EditorGUI::Image (void* nativeTexturePointer, Vector2 drawSize)
	{
		if (nativeTexturePointer != nullptr)
		{
			ImGui::Image (nativeTexturePointer, ImVec2 (drawSize.m_x, drawSize.m_y));
		}
	}

    bool EditorGUI::Button (const std::string& label)
    {
        return ImGui::Button (label.c_str ());
    }

	bool EditorGUI::TreeBeginNode (const std::string& label)
	{
		return ImGui::TreeNode (label.c_str ());
	}

    void EditorGUI::TreePopNode ()
	{
		ImGui::TreePop ();
	}

	void EditorGUI::OpenPopup (const std::string& id)
	{
		ImGui::OpenPopup (id.c_str ());
	}

	bool EditorGUI::BeginPopup (const std::string& id)
	{
		return ImGui::BeginPopup (id.c_str ());
	}

	void EditorGUI::EndPopup ()
	{
		ImGui::EndPopup ();
	}

	bool EditorGUI::BeginPopupContext (const std::string& id)
	{
		return ImGui::BeginPopupContextWindow (id.c_str ());
	}

	void EditorGUI::EndPopupContext ()
	{
		ImGui::EndPopup ();
	}

	bool EditorGUI::InputCheckbox (const std::string& label, bool bChecked)
	{
		InputLabel (label);
		ImGui::Checkbox (InputId (label).c_str (), &bChecked);

		return bChecked;
	}

	int32 EditorGUI::InputInt (const std::string& label, int32 value)
	{
		InputLabel (label);
		ImGui::DragInt (InputId (label).c_str (), &value);

		return value;
	}

	float EditorGUI::InputFloat (const std::string& label, float value)
	{
		InputLabel (label);
		ImGui::DragFloat (InputId (label).c_str (), &value, 0.1f);

		return value;
	}

    std::string EditorGUI::InputText (const std::string& label, const std::string& value)
    {
        std::string input = value;
        InputLabel (label);
        ImGui::InputText (InputId (label).c_str (), &input, ImGuiInputTextFlags_EnterReturnsTrue);
        return input;
    }

	Vector2 EditorGUI::InputVector2 (const std::string& label, Vector2 value)
	{
		InputLabel (label);
		ImGui::DragFloat2 (InputId (label).c_str (), reinterpret_cast<float*> (&value), 0.1f);

		return value;
	}

	Vector3 EditorGUI::InputVector3 (const std::string& label, Vector3 value)
	{
		InputLabel (label);
		ImGui::DragFloat3 (InputId (label).c_str (), reinterpret_cast<float*> (&value), 0.1f);

		return value;
	}

	Vector4 EditorGUI::InputVector4 (const std::string& label, Vector4 value)
	{
		InputLabel (label);
		ImGui::DragFloat4 (InputId (label).c_str (), reinterpret_cast<float*> (&value), 0.1f);

		return value;
	}

	int32 EditorGUI::InputDropDown (const std::string& label, int32 selectedIndex, const std::vector<std::string>& names)
	{
		InputLabel (label);

		if (names.size () > 0)
		{
			int32 nameSize = static_cast<int32> (names.size ());

			if (selectedIndex < 0 || selectedIndex >= nameSize)
			{
				selectedIndex = 0;
			}

			if (ImGui::BeginCombo (InputId (label).c_str (), names.at (selectedIndex).c_str ()))
			{
				for (int32 i = 0; i < nameSize; i++)
				{
					bool bSelected = i == selectedIndex;

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

		return selectedIndex;
	}

	Vector3 EditorGUI::InputColorRGB (const std::string& label, Vector3 rgb)
	{
		ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview;
		InputLabel (label);
		ImGui::ColorEdit3 (InputId (label).c_str (), reinterpret_cast<float*> (&rgb), flags);

		return rgb;
	}

	Vector4 EditorGUI::InputColorRGBA (const std::string& label, Vector4 rgba)
	{
		ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_AlphaPreview;
		InputLabel (label);
		ImGui::ColorEdit4 (InputId (label).c_str (), reinterpret_cast<float*> (&rgba), flags);

		return rgba;
	}

	Object* EditorGUI::InputAsset (const std::string& label, const std::string& type, Object* value)
	{
		InputLabel (label);
		std::string assetName = "None";

		if (value != nullptr)
		{
			PathString name = AssetManager::GetAssetName (*value);
			assetName = std::string (name.begin (), name.end ());
		}

		ImGui::PushID (label.c_str ());

		if (ImGui::BeginCombo ("##SelectAsset", assetName.c_str (), ImGuiComboFlags_NoArrowButton))
		{
			for (std::shared_ptr<Object>& found : AssetManager::FindAssets (type))
			{
				Object* asset = found.get ();
				PathString name = AssetManager::GetAssetName (*asset);
				assetName = std::string (name.begin (), name.end ());

				if (SelectableLabel (assetName, asset == value))
				{
					value = asset;
				}
			}

			ImGui::EndCombo ();
		}

		ImGui::PopID ();

		return value;
	}

	bool EditorGUI::IsStartsWithId (const std::string& string)
	{
		return string.rfind ("##", sizeof ("##")) == 0;
	}

	void EditorGUI::InputLabel (const std::string& label)
	{
		if (IsStartsWithId (label) == false)
		{
			Label (label);
			SameLine ();
		}
	}

	std::string EditorGUI::InputId (const std::string& label)
	{
		return IsStartsWithId (label) ? label : "##" + label;
	}
}
