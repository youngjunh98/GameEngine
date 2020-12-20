#pragma once

#include "Engine/Core/CoreMinimal.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Component.h"

namespace GameEngine
{
	class ENGINE_API Transform : public Component
	{
	public:
		REGISTER_OBJECT_HEADER (Transform)
		
		Transform ();
		virtual ~Transform ();

		void Translate (Vector3 translation);
		void Rotate (Quaternion rotation);

		Vector3 TransformPosition (Vector3 position);
		Vector3 TransformDirection (Vector3 direction);

		Vector3 InverseTransformPosition (Vector3 position);
		Vector3 InverseTransformDirection (Vector3 direction);

		Vector3 Forward () const;
		Vector3 Right () const;
		Vector3 Up () const;

		Vector3 GetPosition () const;
		void SetPosition (Vector3 position);

		Quaternion GetRotation () const;
		void SetRotation (Quaternion rotation);

		Vector3 GetScale () const;
		void SetScale (Vector3 scale);

		virtual void OnRenderEditor () override;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		Vector3 m_position;
		Quaternion m_rotation;
		Vector3 m_scale;
	};
}
