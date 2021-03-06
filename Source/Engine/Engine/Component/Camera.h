#pragma once

#include "Engine/Core/CoreMinimal.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Component.h"

namespace GameEngine
{
	enum class ECameraMode
	{
		Perspective,
		Orthographic
	};

	struct FrustumCorners
	{
		Vector3 m_nearTopLeft;
		Vector3 m_nearTopRight;
		Vector3 m_nearBottomLeft;
		Vector3 m_nearBottomRight;

		Vector3 m_farTopLeft;
		Vector3 m_farTopRight;
		Vector3 m_farBottomLeft;
		Vector3 m_farBottomRight;
	};

	class ENGINE_API Camera : public Component
	{
	public:
		REGISTER_OBJECT_HEADER (Camera)

		Camera ();
		virtual ~Camera ();

		virtual void Destroy () override;

		virtual void OnStart () override;

		float GetAspectRatio () const;
		Matrix4x4 GetViewMatrix () const;
		Matrix4x4 GetProjectionMatrix () const;

		ECameraMode GetMode () const;
		void SetMode (ECameraMode mode);

		float GetFieldOfView () const;
		void SetFieldOfView (float fieldOfView);

		float GetNear () const;
		void SetNear (float zNear);

		float GetFar () const;
		void SetFar (float zFar);

		static FrustumCorners CalculateFrustumCorners (Matrix4x4 viewMatrix, float aspectRatio, float fieldOfView, float nearDistance, float farDistance);

		virtual void OnRenderEditor () override;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	public:
		static Camera * Main;

	private:
		ECameraMode m_mode;
		float m_fov;
		float m_orthographicSize;
		float m_near;
		float m_far;
	};
}
