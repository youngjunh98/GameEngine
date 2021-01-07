#pragma once

#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Physics/Collider.h"

namespace GameEngine
{
	class ENGINE_API BoxCollider : public Collider
	{
	public:
		REGISTER_OBJECT_HEADER (BoxCollider)

		BoxCollider ();
		virtual ~BoxCollider ();

		virtual void OnInit () override;

		void SetSize (Vector3 size);
		Vector3 GetSize () const;

		virtual void OnRenderEditor () override;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	protected:
		virtual physx::PxShape* GetShape () override;

	private:
		Vector3 m_size;
	};
}
