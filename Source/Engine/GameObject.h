#ifndef INCLUDE_GAME_OBJECT
#define INCLUDE_GAME_OBJECT

#include <string>
#include <vector>
#include <memory>
#include <type_traits>

#include "Core/CoreMinimal.h"
#include "Object.h"
#include "Scene/Scene.h"

namespace GameEngine
{
	class GameObject : public Object
	{
	public:
		GameObject (const std::string& name);
		virtual ~GameObject ();

		void Init ();
		virtual void Destroy () override;
		
		void StartComponents ();
		void FixedUpdateComponents ();
		void UpdateComponents ();
		void DeleteDestroyedComponents ();

		template<typename T>
		void AddComponent ()
		{
			static_assert (std::is_base_of<Component, T>::value, "Game object can only add component.");

			auto component = std::make_unique<T> ();
			component->SetGameObject (*this);
			component->OnInit ();

			if (m_scene->IsLoaded ())
			{
				component->OnStart ();
			}

			m_components.push_back (std::move (component));
		}

		template<typename T>
		T* GetComponent () const
		{
			static_assert (std::is_base_of<Component, T>::value, "Game object can only get component");

			for (const auto& component : m_components)
			{
				T* casted = dynamic_cast<T*> (component.get ());

				if (casted != nullptr)
				{
					return casted;
				}
			}

			return nullptr;
		}

		template<typename T>
		std::vector<T*> GetComponents () const
		{
			static_assert (std::is_base_of<Component, T>::value, "Game object can only get component");

			std::vector<T*> foundComponents;

			for (const auto& component : m_components)
			{
				T* casted = dynamic_cast<T*> (component.get ());

				if (casted != nullptr)
				{
					foundComponents.push_back (casted);
				}
			}

			return foundComponents;
		}

		template<>
		std::vector<Component*> GetComponents<Component> () const
		{
			std::vector<Component*> foundComponents;

			for (const auto& component : m_components)
			{
				foundComponents.push_back (component.get ());
			}

			return foundComponents;
		}

		bool IsActive () const;
		void SetActive (bool isActivated);

		std::string GetTag () const;
		void SetTag (const std::string& tag);

		Transform& GetTransform () const;

		void SetScene (Scene* scene);
		Scene* GetScene () const;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		bool m_bActive;
		std::string m_tag;
		std::vector<std::unique_ptr<Component>> m_components;
		Transform* m_transform;
		Scene* m_scene;
	};
}

#endif