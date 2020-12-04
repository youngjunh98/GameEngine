#ifndef INCLUDE_CAMERA
#define INCLUDE_CAMERA

#include "Core/CoreMinimal.h"
#include "Engine/Component/Component.h"

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

	class Camera : public Component
	{
	public:
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

		FrustumCorners CalculateFrustumCorners () const;

		virtual void OnRenderEditor () override;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	public:
		static Camera * Main;

	private:
		ECameraMode m_mode;
		float m_fieldOfView;
		float m_orthographicSize;
		float m_near;
		float m_far;
	};
}

#endif