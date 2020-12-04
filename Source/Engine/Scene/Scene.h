#ifndef INCLUDE_SCENE
#define INCLUDE_SCENE

#include <vector>
#include <memory>

#include "Core/CoreMinimal.h"
#include "Core/JSON/JsonSerializer.h"

namespace GameEngine
{
	class Scene
	{
	public:
		Scene ();
		~Scene ();

		void Start ();
		void FixedUpdate ();
		void Update ();

		void OnSave (Json::Json& sceneData);
		void OnLoad (const Json::Json& sceneData);
		void OnUnload ();

		GameObject* SpawnGameObject ();
		std::vector<std::unique_ptr<GameObject>>& GetGameObjects ();

		bool IsLoaded () const;

	private:
		bool m_bLoaded;
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
	};
}

#endif