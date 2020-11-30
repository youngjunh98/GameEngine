#include "Scene.h"
#include "Engine/GameObject.h"
#include "Engine/Physics/Physics.h"

namespace GameEngine
{
	Scene::Scene () :
		m_gameObjects ()
	{
	}

	Scene::~Scene ()
	{
	}

	void Scene::Start ()
	{
		m_bLoaded = true;

		for (auto& gameObject : m_gameObjects)
		{
			if (gameObject->IsActive () == false)
			{
				continue;
			}

			gameObject->StartComponents ();
		}
	}

	void Scene::FixedUpdate ()
	{
		for (auto& gameObject : m_gameObjects)
		{
			if (gameObject->IsActive () == false)
			{
				continue;
			}

			gameObject->FixedUpdateComponents ();
		}
	}

	void Scene::Update ()
	{
		for (auto& gameObject : m_gameObjects)
		{
			if (gameObject->IsActive () == false)
			{
				continue;
			}

			gameObject->UpdateComponents ();
		}

		auto gameObjectIter = m_gameObjects.begin ();

		while (gameObjectIter != m_gameObjects.end ())
		{
			auto& gameObject = *gameObjectIter;

			if (gameObject->IsAlive ())
			{
				gameObject->DeleteDestroyedComponents ();
				gameObjectIter += 1;

				continue;
			}

			gameObjectIter = m_gameObjects.erase (gameObjectIter);
		}
	}

	void Scene::OnSave (Json::Json& sceneData)
	{
		sceneData["gameObjects"] = Json::Json::array ();

		for (auto& gameObject : m_gameObjects)
		{
			auto serialized = Json::JsonSerializer::Serialize<GameObject> (*gameObject);
			sceneData["gameObjects"].push_back (serialized);
		}

		for (auto& gameObjectData : sceneData["gameObjects"])
		{
			std::string string = gameObjectData.dump ();
		}
	}

	void Scene::OnLoad (const Json::Json& sceneData)
	{
		if (sceneData.find ("gameObjects") != sceneData.end ())
		{
			for (auto& gameObjectData : sceneData["gameObjects"].items ())
			{
				auto* gameObject = SpawnGameObject ();
				gameObject->OnDeserialize (gameObjectData.value ());
			}
		}

		g_physics.CreatePhysicScene ();
	}

	void Scene::OnUnload ()
	{
		if (m_gameObjects.size () > 0)
		{
			for (auto& gameObject : m_gameObjects)
			{
				gameObject->Destroy ();
			}

			m_gameObjects.clear ();
		}

		g_physics.DeletePhysicScene ();
	}

	GameObject* Scene::SpawnGameObject ()
	{
		GameObject* temp = nullptr;
		auto gameObject = std::make_unique<GameObject> ("New Game Object");

		if (gameObject != nullptr)
		{
			gameObject->SetScene (this);
			gameObject->Init ();

			temp = gameObject.get ();
			m_gameObjects.push_back (std::move (gameObject));
		}

		return temp;
	}

	std::vector<std::unique_ptr<GameObject>>& Scene::GetGameObjects ()
	{
		return m_gameObjects;
	}

	bool Scene::IsLoaded () const
	{
		return m_bLoaded;
	}
}