#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Type.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Engine/EngineMacro.h"

namespace GameEngine
{
	class Component;
	class Scene;
	class Transform;

	class ENGINE_API GameObject : public Object
	{
	public:
		REGISTER_OBJECT_HEADER (GameObject)

		GameObject (const std::string& name = "Game Object");
		virtual ~GameObject ();

		void Init ();
		virtual void Destroy () override;
		
		void StartComponents ();
		void FixedUpdateComponents ();
		void UpdateComponents ();
		void DeleteDestroyedComponents ();

		template<typename TComponent>
		void AddComponent ()
		{
			static_assert (IsBaseOf<Component, TComponent>::Result, "Game object can only add component.");
			AddComponentInternal (std::make_shared<TComponent> ());
		}

		template<typename TComponent>
		TComponent* GetComponent () const
		{
			static_assert (IsBaseOf<Component, TComponent>::Result, "Game object can only get component");

			for (const auto& component : m_components)
			{
				TComponent* casted = dynamic_cast<TComponent*> (component.get ());

				if (casted != nullptr)
				{
					return casted;
				}
			}

			return nullptr;
		}

		template<typename TComponent>
		std::vector<TComponent*> GetComponents () const
		{
			static_assert (IsBaseOf<Component, TComponent>::Result, "Game object can only get component");

			std::vector<TComponent*> foundComponents;

			for (const auto& component : m_components)
			{
				TComponent* casted = dynamic_cast<TComponent*> (component.get ());

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
		void AddComponentInternal (std::shared_ptr<Component> component);

	private:
		bool m_bActive;
		std::string m_tag;
		std::vector<std::shared_ptr<Component>> m_components;
		Transform* m_transform;
		Scene* m_scene;
	};
}
