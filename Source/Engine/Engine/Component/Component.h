#pragma once

#include <string>

#include "Type.h"
#include "Engine/Core/Object.h"
#include "Engine/Engine/EngineMacro.h"

namespace GameEngine
{
	class GameObject;

	class ENGINE_API Component : public Object
	{
	public:
		Component (const std::string& name = "Component");
		virtual ~Component () = 0;

		virtual void Destroy () override;

		virtual void OnInit () {}
		virtual void OnStart () {}
		virtual void OnFixedUpdate () {}
		virtual void OnUpdate () {}

		virtual void OnPreRender () {}
		virtual void OnPostRender () {}

		virtual void OnCollisionStart () {}
		virtual void OnCollisionFinish () {}
		virtual void OnCollisionStay () {}

		virtual void OnTriggerStart () {}
		virtual void OnTriggerFinish () {}
		virtual void OnTriggerStay () {}

		GameObject& GetGameObject () const;
		void SetGameObject (GameObject& gameObject);

	private:
		GameObject* m_gameObject;
	};
}
