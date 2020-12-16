#include "GameObject.h"
#include "Engine/Engine/Scene/Scene.h"
#include "Component/Component.h"
#include "Component/Transform.h"
#include "Component/Camera.h"
#include "Component/Rendering/MeshRenderer.h"
#include "Component/Rendering/Light.h"
#include "Component/Physics/Rigidbody.h"
#include "Component/Physics/BoxCollider.h"
#include "Component/Physics/SphereCollider.h"

namespace GameEngine
{
	GameObject::GameObject (const std::string& name) : Object (name),
		m_bActive (true), m_tag ("Untagged"), m_components (), m_transform (nullptr), m_scene (nullptr)
	{
	}

	GameObject::~GameObject ()
	{
	}

	void GameObject::Init ()
	{
		AddComponent<Transform> ();
		m_transform = GetComponent<Transform> ();
	}

	void GameObject::Destroy ()
	{
		Object::Destroy ();

		for (auto& component : m_components)
		{
			component->Destroy ();
		}

		m_tag.clear ();
		m_components.clear ();

		m_bActive = false;
	}

	void GameObject::StartComponents ()
	{
		for (auto& component : m_components)
		{
			component->OnStart ();
		}
	}

	void GameObject::FixedUpdateComponents ()
	{
		for (auto& component : m_components)
		{
			component->OnFixedUpdate ();
		}
	}

	void GameObject::UpdateComponents ()
	{
		for (auto& component : m_components)
		{
			component->OnUpdate ();
		}
	}

	void GameObject::DeleteDestroyedComponents ()
	{
		auto componentIterator = m_components.begin ();

		while (componentIterator != m_components.end ())
		{
			if ((*componentIterator)->IsAlive () == false)
			{
				componentIterator = m_components.erase (componentIterator);
			}
			else
			{
				componentIterator += 1;
			}
		}
	}

	bool GameObject::IsActive () const
	{
		return m_bActive;
	}

	void GameObject::SetActive (bool isActivated)
	{
		m_bActive = isActivated;
	}

	std::string GameObject::GetTag () const
	{
		return m_tag;
	}

	void GameObject::SetTag (const std::string& tag)
	{
		m_tag = tag;
	}

	Transform& GameObject::GetTransform () const
	{
		return *m_transform;
	}

	void GameObject::SetScene (Scene* scene)
	{
		m_scene = scene;
	}

	Scene* GameObject::GetScene () const
	{
		return m_scene;
	}

	void GameObject::OnSerialize (Json::Json& json) const
	{
		Object::OnSerialize (json);
		
		json["type"] = "GameObject";
		json["tag"] = m_tag;
		json["components"] = Json::Json::array ();

		for (auto& component : m_components)
		{
			auto serialized = Json::JsonSerializer::Serialize<Component> (*component);
			json["components"].push_back (serialized);
		}
	}

	void GameObject::OnDeserialize (const Json::Json& json)
	{
		Object::OnDeserialize (json);

		json.at ("tag").get_to (m_tag);

		if (json.find ("components") != json.end ())
		{
			for (auto& component : json.at ("components"))
			{
				if (component["type"] == "Transform")
				{
					m_components.at (0)->OnDeserialize (component);
					continue;
				}
				else if (component["type"] == "Camera")
				{
					AddComponent<Camera> ();
				}
				else if (component["type"] == "MeshRenderer")
				{
					AddComponent<MeshRenderer> ();
				}
				else if (component["type"] == "Light")
				{
					AddComponent<Light> ();
				}
				else if (component["type"] == "Rigidbody")
				{
					AddComponent<Rigidbody> ();
				}
				else if (component["type"] == "Box Collider")
				{
					AddComponent<BoxCollider> ();
				}
				else if (component["type"] == "Sphere Collider")
				{
					AddComponent<SphereCollider> ();
				}

				m_components.at (m_components.size () - 1)->OnDeserialize (component);
			}
		}
	}

	void GameObject::AddComponentInternal (std::shared_ptr<Component> component)
	{
		component->SetGameObject (*this);
		component->OnInit ();

		if (m_scene->IsLoaded ())
		{
			component->OnStart ();
		}

		m_components.push_back (component);
	}
}
