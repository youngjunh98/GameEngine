#ifndef INCLUDE_COMPONENT
#define INCLUDE_COMPONENT

#include "Core/CoreMinimal.h"
#include "Engine/Object.h"

namespace GameEngine
{
	class Component : public Object
	{
	public:
		Component ();
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

#endif