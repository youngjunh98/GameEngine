#ifndef INCLUDE_LIGHT
#define INCLUDE_LIGHT


#include "Core/CoreMinimal.h"
#include "Component/Component.h"

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

	class Light : public Component
	{
	public:
		Light () :
			m_type (ELightType::Directional),
			m_color (Vector4::One),
			m_intensity (1.0f),
			m_range (0.0f),
			m_spotAngle (0.0f),
			m_shadowType (EShadowType::Hard),
			m_shadowIntensity (1.0f),
			m_shadowDepthBias (0.001f)
		{}
		virtual ~Light () {}

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

		virtual void OnRenderEditor (Editor& editor) override;

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

#endif