#pragma once

#include <vector>
#include <memory>

#include "Type.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Engine/Engine/EngineMacro.h"

namespace GameEngine
{
	class GameObject;

	class ENGINE_API Scene
	{
	public:
		Scene ();
		~Scene ();

		void Start ();
		void FixedUpdate ();
		void Update ();

		void OnSave (Json::Json& sceneJson);
		void OnLoad (const Json::Json& sceneJson);
		void OnUnload ();

		GameObject* SpawnGameObject ();
		std::vector<std::shared_ptr<GameObject>>& GetGameObjects ();

		bool IsLoaded () const;

	private:
		bool m_bLoaded;
		std::vector<std::shared_ptr<GameObject>> m_gameObjects;
	};
}
