#include "SphereCollider.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (SphereCollider)

	SphereCollider::SphereCollider () : Collider ("Sphere Collider"),
		m_radius (1.0f)
	{
	}

	SphereCollider::~SphereCollider ()
	{
	}

	void SphereCollider::OnInit ()
	{
		Collider::OnInit ();

		SetRadius (m_radius);
	}

	void SphereCollider::SetRadius (float radius)
	{
		m_radius = radius;
		physx::PxSphereGeometry sphereGeometry (radius);
		GetShape ()->setGeometry (sphereGeometry);
	}

	float SphereCollider::GetRadius () const
	{
		return m_radius;
	}

	void SphereCollider::OnRenderEditor ()
	{
		float radius = Math::Max (EditorGUI::InputFloat ("Size", m_radius), 0.0f);
		Vector3 offset = EditorGUI::InputVector3 ("Offset", m_offset);

		SetRadius (radius);
		SetOffset (offset);
	}

	void SphereCollider::OnSerialize (Json::Json& json) const
	{
		Collider::OnSerialize (json);
		Json::JsonSerializer::Serialize (json, "radius", m_radius);
	}

	void SphereCollider::OnDeserialize (const Json::Json& json)
	{
		Collider::OnDeserialize (json);
		SetRadius (Json::JsonSerializer::Deserialize<float> (json, "radius"));
	}

	physx::PxShape* SphereCollider::GetShape ()
	{
		if (m_physxShape == nullptr)
		{
			g_physics.CreateSphereCollider (*this);
		}

		return m_physxShape;
	}
}