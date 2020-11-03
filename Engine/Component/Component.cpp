#include "Component/Component.h"

namespace GameEngine
{
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
