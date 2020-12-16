#pragma once

#include <string>
#include <vector>

#include "Type.h"
#include "Editor/Core/EditorCoreMacro.h"
#include "Engine/Core/Math/Vector2.h"
#include "Engine/Core/Math/Vector3.h"
#include "Engine/Core/Math/Vector4.h"

namespace GameEngine
{
    class Object;

    class EDITOR_CORE_API EditorGUI final
    {
    private:
        EditorGUI ();
        virtual ~EditorGUI () = 0;

    public:
        static void PushID (int32 id);
        static void PushID (const std::string& id);
        static void PopID ();

        static bool IsWindowFocused ();
        static bool IsWindowHovered ();

        static bool IsLastItemClicked ();
        static bool IsLastItemFocused ();
        static bool IsLastItemHovered ();

        static Vector2 GetWindowSize ();
        static Vector2 GetWindowContentRegionSize ();
        static Vector2 GetWindowAvailableContentRegionSize ();

        static void SameLine ();
        static void Separator ();

        static void Label (const std::string& label);
        static bool SelectableLabel (const std::string& label, bool bSelected, bool bDisabled = false);
        static void Image (void* nativeTexturePointer, Vector2 drawSize);
        static bool Button (const std::string& label);

        static bool TreeBeginNode (const std::string& label);
        static void TreePopNode ();

        static void OpenPopup (const std::string& id);
        static bool BeginPopup (const std::string& id);
        static void EndPopup ();
        static bool BeginPopupContext (const std::string& id);
        static void EndPopupContext ();

        static bool InputCheckbox (const std::string& label, bool bChecked);
        static int32 InputInt (const std::string& label, int32 value);
        static float InputFloat (const std::string& label, float value);
        static std::string InputText (const std::string& label, const std::string& value);
        static Vector2 InputVector2 (const std::string& label, Vector2 value);
        static Vector3 InputVector3 (const std::string& label, Vector3 value);
        static Vector4 InputVector4 (const std::string& label, Vector4 value);
        static int32 InputDropDown (const std::string& label, int32 selectedIndex, const std::vector<std::string>& names);
        static Vector3 InputColorRGB (const std::string& label, Vector3 rgb);
        static Vector4 InputColorRGBA (const std::string& label, Vector4 rgba);
        static Object* InputAsset (const std::string& label, const std::string& type,  Object* value);

    private:
        static bool IsStartsWithId (const std::string& string);
        static void InputLabel (const std::string& label);
        static std::string InputId (const std::string& label);
    };
}
