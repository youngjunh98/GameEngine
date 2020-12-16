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

	void SceneManager::LoadScene (const std::wstring& path)
	{
		auto sceneDataIter = m_sceneDataMap.find (path);
		bool bPathInvalid = sceneDataIter == m_sceneDataMap.end ();

		if (bPathInvalid)
		{
			return;
		}

		if (m_scene != nullptr)
		{
			UnloadScene ();
		}

		m_scene = std::make_unique<Scene> ();
		m_scenePath = path;

		m_scene->OnLoad (sceneDataIter->second);
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

	void SceneManager::CreateEmptyScene (const std::wstring& path)
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

	void SceneManager::RegisterScene (const std::wstring& path, const Json::Json& sceneData)
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

	std::wstring SceneManager::GetScenePath ()
	{
		return m_scenePath;
	}

	Json::Json SceneManager::FindSceneData (const std::wstring& path)
	{
		Json::Json found;

		auto sceneDataIter = m_sceneDataMap.find (path);
		bool bFound = sceneDataIter != m_sceneDataMap.end ();

		if (bFound)
		{
			found = sceneDataIter->second;
		}

		return found;
	}
}