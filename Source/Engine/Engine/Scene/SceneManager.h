#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Scene/Scene.h"

namespace GameEngine
{
	class ENGINE_API SceneManager final
	{
	private:
		SceneManager ();
		~SceneManager ();

	public:
		SceneManager (const SceneManager&) = delete;
		SceneManager& operator= (const SceneManager&) = delete;
		
		static SceneManager& GetInstance ();

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
}
