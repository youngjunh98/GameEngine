#pragma once

#include <memory>
#include <unordered_map>

#include "Engine/Core/File/FileSystem.h"
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
		void LoadScene (const PathString& path);
		void UnloadScene ();

		void CreateEmptyScene (const PathString& path);
		void RegisterScene (const PathString& path, const Json::Json& sceneData);

		Scene* GetScene ();
		PathString GetScenePath ();
		bool FindSceneData (const PathString& path, Json::Json& sceneJson);

	private:
		std::unique_ptr<Scene> m_scene;
		PathString m_scenePath;

		std::unordered_map<PathString, Json::Json> m_sceneDataMap;
	};
}
