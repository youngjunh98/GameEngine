#include "BoxCollider.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Editor/Core/EditorGUI.h"
#include "Engine/Engine/Physics/Physics.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (BoxCollider)

	BoxCollider::BoxCollider () : Collider ("Box Collider"),
		m_size (Vector3::One)
	{
	}

	BoxCollider::~BoxCollider ()
	{
	}

	void BoxCollider::OnInit ()
	{
		Collider::OnInit ();

		SetSize (m_size);
	}

	void BoxCollider::SetSize (Vector3 size)
	{
		m_size = size;
		physx::PxBoxGeometry boxGeometry (0.5f * size.m_x, 0.5f * size.m_y, 0.5f * size.m_z);
		GetShape ()->setGeometry (boxGeometry);
	}

	Vector3 BoxCollider::GetSize () const
	{
		return m_size;
	}

	void BoxCollider::OnRenderEditor ()
	{
		Vector3 size = EditorGUI::InputVector3 ("Size", m_size);
		size.m_x = Math::Max (0.0f, size.m_x);
		size.m_y = Math::Max (0.0f, size.m_y);
		size.m_z = Math::Max (0.0f, size.m_z);

		Vector3 offset = EditorGUI::InputVector3 ("Offset", m_offset);

		SetSize (size);
		SetOffset (offset);
	}

	void BoxCollider::OnSerialize (Json::Json& json) const
	{
		Collider::OnSerialize (json);
		Json::JsonSerializer::Serialize (json, "size", m_size);
	}

	void BoxCollider::OnDeserialize (const Json::Json& json)
	{
		Collider::OnDeserialize (json);
		SetSize (Json::JsonSerializer::Deserialize<Vector3> (json, "size"));
	}

	physx::PxShape* BoxCollider::GetShape ()
	{
		if (m_physxShape == nullptr)
		{
			g_physics.CreateBoxCollider (*this);
		}

		return m_physxShape;
	}
}
