#include "SceneManager.h"
#include "Engine/Core/File/File.h"

namespace GameEngine
{
	SceneManager::SceneManager ()
	{
	}

	SceneManager::~SceneManager ()
	{
	}

	SceneManager& SceneManager::GetInstance ()
	{
		static SceneManager instance;
		return instance;
	}

	void SceneManager::FixedUpdateScene ()
	{
		if (m_scene == nullptr)
		{
			return;
		}

		m_scene->FixedUpdate ();
	}

	void SceneManager::UpdateScene ()
	{
		if (m_scene == nullptr)
		{
			return;
		}
		
		m_scene->Update ();
	}

	void SceneManager::SaveScene ()
	{
		File file (m_scenePath, EFileAccessMode::Write);

		if (file.IsOpen () == false)
		{
			return;
		}

		Json::Json sceneData;
		m_scene->OnSave (sceneData);

		RegisterScene (m_scenePath, sceneData);

		std::string jsonString = sceneData.dump ();
		int64 jsonStringSize = jsonString.size ();

		file.Write (jsonString.data (), jsonStringSize);
	}

	void SceneManager::LoadScene (const PathString& path)
	{
		Json::Json sceneJson;

		if (FindSceneData (path, sceneJson) == false)
		{
			File file (path, EFileAccessMode::Read);
			int64 fileSize = file.GetSize ();

			std::string jsonString (fileSize + 1, L'\0');
			auto* first = const_cast<char*> (jsonString.data ());

			file.ReadAll (first);
			sceneJson = Json::Json::parse (jsonString);

			RegisterScene (path, sceneJson);
		}

		if (m_scene != nullptr)
		{
			UnloadScene ();
		}

		m_scene = std::make_unique<Scene> ();
		m_scenePath = path;

		m_scene->OnLoad (sceneJson);
		m_scene->Start ();
	}

	void SceneManager::UnloadScene ()
	{
		if (m_scene != nullptr)
		{
			m_scene->OnUnload ();
			m_scene = nullptr;
		}

		m_scenePath = L"";
	}

	void SceneManager::CreateEmptyScene (const PathString& path)
	{
		File file (path, EFileAccessMode::Write);

		if (file.IsOpen () == false)
		{
			return;
		}

		Json::Json emptySceneData = Json::Json::object ();
		RegisterScene (path, emptySceneData);

		std::string jsonString = emptySceneData.dump ();
		int64 jsonStringSize =jsonString.size ();

		file.Write (jsonString.data (), jsonStringSize);

		LoadScene (path);
	}

	void SceneManager::RegisterScene (const PathString& path, const Json::Json& sceneData)
	{
		auto foundIter = m_sceneDataMap.find (path);
		bool bFound = foundIter != m_sceneDataMap.end ();

		if (bFound)
		{
			foundIter->second = sceneData;
		}
		else
		{
			m_sceneDataMap.emplace (path, sceneData);
		}
	}

	Scene* SceneManager::GetScene ()
	{
		return m_scene.get ();
	}

	PathString SceneManager::GetScenePath ()
	{
		return m_scenePath;
	}

	bool SceneManager::FindSceneData (const PathString& path, Json::Json& sceneJson)
	{
		auto sceneDataIter = m_sceneDataMap.find (path);
		bool bFound = sceneDataIter != m_sceneDataMap.end ();

		if (bFound)
		{
			sceneJson = sceneDataIter->second;
		}

		return bFound;
	}
}