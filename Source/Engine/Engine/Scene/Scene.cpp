#include "Scene.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Engine/GameObject.h"
#include "Engine/Engine/Physics/Physics.h"

namespace GameEngine
{
	Scene::Scene () :
		m_bLoaded (false), m_gameObjects ()
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

	void Scene::OnSave (Json::Json& sceneJson)
	{
		Json::JsonSerializer::CreateArray (sceneJson, "gameObjects");

		for (const std::shared_ptr<GameObject>& gameObject : m_gameObjects)
		{
			Json::Json gameObjectJson = Json::JsonSerializer::ObjectToJson (*gameObject);
			Json::JsonSerializer::AppendArray (sceneJson, "gameObjects", gameObjectJson);
		}
	}

	void Scene::OnLoad (const Json::Json& sceneJson)
	{
		g_physics.CreatePhysicScene ();

		for (auto it = Json::JsonSerializer::GetArrayBegin (sceneJson, "gameObjects"); it != Json::JsonSerializer::GetArrayEnd (sceneJson, "gameObjects"); it++)
		{
			Json::Json gameObjectJson = it.value ();

			auto* gameObject = SpawnGameObject ();
			gameObject->OnDeserialize (gameObjectJson);
		}
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
		auto gameObject = std::make_shared<GameObject> ("New Game Object");

		if (gameObject != nullptr)
		{
			gameObject->SetScene (this);
			gameObject->Init ();
			m_gameObjects.push_back (gameObject);
		}

		return gameObject.get ();
	}

	std::vector<std::shared_ptr<GameObject>>& Scene::GetGameObjects ()
	{
		return m_gameObjects;
	}

	bool Scene::IsLoaded () const
	{
		return m_bLoaded;
	}
}