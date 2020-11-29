#include "Component.h"

namespace GameEngine
{
	Component::Component () : m_gameObject (nullptr)
	{
	}

	Component::~Component ()
	{
	}

	void Component::Destroy ()
	{
		Object::Destroy ();

		m_gameObject = nullptr;
	}

	GameObject& Component::GetGameObject () const
	{
		return *m_gameObject;
	}

	void Component::SetGameObject (GameObject& gameObject)
	{
		m_gameObject = &gameObject;
	}
}
