#pragma once

#include "Engine/Core/CoreMinimal.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Component/Component.h"

namespace GameEngine
{
	enum class ELightType
	{
		Directional,
		Point,
		Spot
	};

	enum class EShadowType
	{
		None,
		Hard,
		Soft
	};

	class ENGINE_API Light : public Component
	{
	public:
		REGISTER_OBJECT_HEADER (Light)

		Light ();
		virtual ~Light ();

		ELightType GetType () const;
		void SetType (ELightType type);

		Vector4 GetColor () const;
		void SetColor (Vector4 color);

		float GetIntensity () const;
		void SetIntensity (float intensity);

		float GetRange () const;
		void SetRange (float range);

		float GetSpotAngle () const;
		void SetSpotAngle (float angle);

		EShadowType GetShadowType () const;
		void SetShadowType (EShadowType shadowType);

		float GetShadowIntensity () const;
		void SetShadowIntensity (float shadowIntensity);

		float GetShadowDepthBias () const;
		void SetShadowDepthBias (float shadowDepthBias);

		virtual void OnRenderEditor () override;

		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		ELightType m_type;
		Vector4 m_color;
		float m_intensity;
		float m_range;
		float m_spotAngle;
		EShadowType m_shadowType;
		float m_shadowIntensity;
		float m_shadowDepthBias;
	};
}
