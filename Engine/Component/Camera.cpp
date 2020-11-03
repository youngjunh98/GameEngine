#include "Screen.h"
#include "GameObject.h"
#include "Component/Camera.h"
#include "Component/Transform.h"
#include "Rendering/GlobalRenderer.h"

namespace GameEngine
{
	Camera* Camera::Main = nullptr;

	Camera::Camera () :
		m_mode (ECameraMode::Perspective),
		m_fieldOfView (60.0f),
		m_orthographicSize (5.0f),
		m_near (0.1f),
		m_far (1000.0f)
	{
	}

	Camera::~Camera ()
	{
	}

	void Camera::Destroy ()
	{
		Component::Destroy ();

		if (this == Main)
		{
			Main = nullptr;
		}
	}

	void Camera::OnStart ()
	{
		if (Main == nullptr)
		{
			Main = this;
		}
	}

	Matrix4x4 Camera::GetViewMatrix () const
	{
		auto& transform = GetGameObject ().GetTransform ();
		Vector3 position = transform.GetPosition ();
		Vector3 lookAt = position + transform.Forward ();
		Vector3 up = transform.Up ();

		return Matrix4x4::LookAt (position, lookAt, up);
	}

	Matrix4x4 Camera::GetProjectionMatrix () const
	{
		GlobalRendererSettings settings = g_renderer.GetSettings ();
		float aspectRatio = static_cast<float> (settings.m_renderWidth) / settings.m_renderHeight;

		if (m_mode == ECameraMode::Perspective)
		{
			return Matrix4x4::Perspective (m_fieldOfView, aspectRatio, m_near, m_far);
		}
		else
		{
			float width = m_orthographicSize * aspectRatio;
			float height = m_orthographicSize;

			return Matrix4x4::Orthographic (width, height, m_near, m_far);
			//return Matrix4x4::Orthographic (static_cast<float> (Screen::Width ()), static_cast<float> (Screen::Height ()), m_near, m_far);
		}
	}

	ECameraMode Camera::GetMode () const
	{
		return m_mode;
	}

	void Camera::SetMode (ECameraMode projection)
	{
		m_mode = projection;
	}

	float Camera::GetFieldOfView () const
	{
		return m_fieldOfView;
	}

	void Camera::SetFieldOfView (float fieldOfView)
	{
		m_fieldOfView = fieldOfView;
	}

	float Camera::GetNear () const
	{
		return m_near;
	}

	void Camera::SetNear (float zNear)
	{
		m_near = zNear;
	}

	float Camera::GetFar () const
	{
		return m_far;
	}

	void Camera::SetFar (float zFar)
	{
		m_far = zFar;
	}

	FrustumCorners Camera::CalculateFrustumCorners () const
	{
		FrustumCorners corners;

		Matrix4x4 inverseView = GetViewMatrix ().Inversed ();
		float tangentHalfFOV = Math::Tan (0.5f * m_fieldOfView * Math::Deg2Rad);
		float aspectRatio = Screen::AspectRatio ();

		float nearHalfHeight = tangentHalfFOV * m_near;
		float nearHalfWidth = nearHalfHeight * aspectRatio;

		Vector4 nearTopLeft = Vector4 (-nearHalfWidth, nearHalfHeight, m_near, 1.0f) * inverseView;
		Vector4 nearTopRight = Vector4 (nearHalfWidth, nearHalfHeight, m_near, 1.0f) * inverseView;
		Vector4 nearBottomLeft = Vector4 (-nearHalfWidth, -nearHalfHeight, m_near, 1.0f) * inverseView;
		Vector4 nearBottomRight = Vector4 (nearHalfWidth, -nearHalfHeight, m_near, 1.0f) * inverseView;
		
		corners.m_nearTopLeft = Vector3 (nearTopLeft.m_x, nearTopLeft.m_y, nearTopLeft.m_z);
		corners.m_nearTopRight = Vector3 (nearTopRight.m_x, nearTopRight.m_y, nearTopRight.m_z);
		corners.m_nearBottomLeft = Vector3 (nearBottomLeft.m_x, nearBottomLeft.m_y, nearBottomLeft.m_z);
		corners.m_nearBottomRight = Vector3 (nearBottomRight.m_x, nearBottomRight.m_y, nearBottomRight.m_z);

		float farHalfHeight = tangentHalfFOV * m_far;
		float farHalfWidth = farHalfHeight * aspectRatio;

		Vector4 farTopLeft = Vector4 (-farHalfWidth, farHalfHeight, m_far, 1.0f) * inverseView;
		Vector4 farTopRight = Vector4 (farHalfWidth, farHalfHeight, m_far, 1.0f) * inverseView;
		Vector4 farBottomLeft = Vector4 (-farHalfWidth, -farHalfHeight, m_far, 1.0f) * inverseView;
		Vector4 farBottomRight = Vector4 (farHalfWidth, -farHalfHeight, m_far, 1.0f) * inverseView;

		corners.m_farTopLeft = Vector3 (farTopLeft.m_x, farTopLeft.m_y, farTopLeft.m_z);
		corners.m_farTopRight = Vector3 (farTopRight.m_x, farTopRight.m_y, farTopRight.m_z);
		corners.m_farBottomLeft = Vector3 (farBottomLeft.m_x, farBottomLeft.m_y, farBottomLeft.m_z);
		corners.m_farBottomRight = Vector3 (farBottomRight.m_x, farBottomRight.m_y, farBottomRight.m_z);

		return corners;
	}

	void Camera::OnRenderEditor (Editor& editor)
	{
		editor.BeginComponent ("Camera", this, true);

		int32 mode = static_cast<int32> (m_mode);
		editor.AddCombo ("Mode", mode, { "Perspective", "Orthographic" });
		m_mode = static_cast<ECameraMode> (mode);

		if (m_mode == ECameraMode::Perspective)
		{
			editor.AddPropertyFloat ("FOV", m_fieldOfView);
			m_fieldOfView = Math::Clamp (m_fieldOfView, 1.0f, 179.0f);
		}
		else
		{
			editor.AddPropertyFloat ("Orthographic Size", m_orthographicSize);
			m_orthographicSize = Math::Max (m_orthographicSize, 0.0001f);
		}

		editor.AddPropertyFloat ("Near", m_near);
		m_near = Math::Max (m_near, 0.0001f);

		editor.AddPropertyFloat ("Far", m_far);
		m_far = Math::Max (m_far, m_near + Math::Epsilon);
	}

	void Camera::OnSerialize (Json::Json& json) const
	{
		Component::OnSerialize (json);

		json["type"] = "Camera";
		json["mode"] = static_cast<int32> (m_mode);
		json["fov"] = m_fieldOfView;
		json["ortho size"] = m_orthographicSize;
		json["near"] = m_near;
		json["far"] = m_far;
	}

	void Camera::OnDeserialize (const Json::Json& json)
	{
		Component::OnDeserialize (json);

		int32 mode;
		json.at ("mode").get_to (mode);
		m_mode = static_cast<ECameraMode> (mode);

		json.at ("fov").get_to (m_fieldOfView);
		json.at ("ortho size").get_to (m_orthographicSize);
		json.at ("near").get_to (m_near);
		json.at ("far").get_to (m_far);
	}
}
