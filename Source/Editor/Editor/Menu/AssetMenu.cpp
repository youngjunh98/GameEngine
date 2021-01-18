#include "AssetMenu.h"
#include "Editor/Editor/Editor.h"
#include "Editor/Editor/Modal/ImportAssetModal.h"

namespace GameEngine
{
	AssetMenu::AssetMenu () : EditorMenu ("Asset")
	{
	}

	AssetMenu::~AssetMenu ()
	{
	}

	void AssetMenu::OnRender ()
	{
		if (MenuItem ("Import"))
		{
			Editor::GetInstance ().OpenModalWindow<ImportAssetModal> ();
		}
	}
}
