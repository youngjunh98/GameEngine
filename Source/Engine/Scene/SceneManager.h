#ifndef INCLUDE_SCENE_MANAGER
#define INCLUDE_SCENE_MANAGER

#include <string>
#include <memory>
#include <unordered_map>

#include "Core/JSON/JsonSerializer.h"
#include "Engine/Scene/Scene.h"

namespace GameEngine
{
	class SceneManager
	{
	public:
		SceneManager ();
		~SceneManager ();

		void FixedUpdateScene ();
		void UpdateScene ();

		void SaveScene ();
		void LoadScene (const std::wstring& path);
		void UnloadScene ();

		void CreateEmptyScene (const std::wstring& path);
		void RegisterScene (const std::wstring& path, const Json::Json& sceneData);

		Scene* GetScene ();
		std::wstring GetScenePath ();
		Json::Json FindSceneData (const std::wstring& path);

	private:
		std::unique_ptr<Scene> m_scene;
		std::wstring m_scenePath;

		std::unordered_map<std::wstring, Json::Json> m_sceneDataMap;
	};

	extern SceneManager g_sceneManager;
}

#endif