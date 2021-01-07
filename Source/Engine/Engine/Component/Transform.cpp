#include "Transform.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (Transform)

	Transform::Transform () : Component ("Transform"),
		m_position (Vector3::Zero), m_rotation (Quaternion::Identity), m_scale (Vector3::One)
	{
	}

	Transform::~Transform ()
	{
	}

	void Transform::Translate (Vector3 translation)
	{
		m_position += translation;
	}

	void Transform::Rotate (Quaternion rotation)
	{
		m_rotation = m_rotation * rotation;
	}

	Vector3 Transform::TransformPosition (Vector3 position)
	{
		return Matrix4x4::ScaleRotateTranslate (m_position, m_rotation, m_scale).TransformPosition (position);
	}

	Vector3 Transform::TransformDirection (Vector3 direction)
	{
		return m_rotation * direction;
	}

	Vector3 Transform::InverseTransformPosition (Vector3 position)
	{
		float inverseScaleX = m_scale.m_x > Math::Epsilon ? m_scale.m_x : 0.0f;
		float inverseScaleY = m_scale.m_y > Math::Epsilon ? m_scale.m_y : 0.0f;
		float inverseScaleZ = m_scale.m_z > Math::Epsilon ? m_scale.m_z : 0.0f;

		Matrix4x4 inverseTranslate = Matrix4x4::Translate (-m_position);
		Matrix4x4 inverseRotate = Matrix4x4::Rotate (m_rotation.Inversed ());
		Matrix4x4 inverseScale = Matrix4x4::Scale (Vector3 (inverseScaleX, inverseScaleY, inverseScaleZ));

		return (inverseTranslate * inverseRotate * inverseScale).TransformPosition (position);
	}

	Vector3 Transform::InverseTransformDirection (Vector3 direction)
	{
		return m_rotation.Inversed () * direction;
	}

	Vector3 Transform::Forward () const
	{
		return m_rotation * Vector3::Forward;
	}

	Vector3 Transform::Right () const
	{
		return m_rotation * Vector3::Right;
	}

	Vector3 Transform::Up () const
	{
		return m_rotation * Vector3::Up;
	}

	Vector3 Transform::GetPosition () const
	{
		return m_position;
	}

	void Transform::SetPosition (Vector3 position)
	{
		m_position = position;
	}

	Quaternion Transform::GetRotation () const
	{
		return m_rotation;
	}

	void Transform::SetRotation (Quaternion rotation)
	{
		m_rotation = rotation;
	}

	Vector3 Transform::GetScale () const
	{
		return m_scale;
	}

	void Transform::SetScale (Vector3 scale)
	{
		m_scale = scale;
	}

	void Transform::OnRenderEditor ()
	{
		m_position = EditorGUI::InputVector3 ("Position", m_position);
		m_rotation = Quaternion::FromEuler (EditorGUI::InputVector3 ("Rotation", m_rotation.Euler ()));
		m_scale = EditorGUI::InputVector3 ("Scale", m_scale);
	}

	void Transform::OnSerialize (Json::Json& json) const
	{
		Component::OnSerialize (json);

		Json::JsonSerializer::Serialize<Vector3> (json, "position", m_position);
		Json::JsonSerializer::Serialize<Quaternion> (json, "rotation", m_rotation);
		Json::JsonSerializer::Serialize<Vector3> (json, "scale", m_scale);
	}

	void Transform::OnDeserialize (const Json::Json& json)
	{
		Component::OnDeserialize (json);

		m_position = Json::JsonSerializer::Deserialize<Vector3> (json, "position");
		m_rotation = Json::JsonSerializer::Deserialize<Quaternion> (json, "rotation");
		m_scale = Json::JsonSerializer::Deserialize<Vector3> (json, "scale");
	}
}
