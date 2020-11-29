#include "Light.h"

namespace GameEngine
{
	ELightType Light::GetType () const
	{
		return m_type;
	}

	void Light::SetType (ELightType type)
	{
		m_type = type;
	}

	Vector4 Light::GetColor () const
	{
		return m_color;
	}

	void Light::SetColor (Vector4 color)
	{
		m_color = color;
	}

	float Light::GetIntensity () const
	{
		return m_intensity;
	}

	void Light::SetIntensity (float intensity)
	{
		m_intensity = intensity;
	}

	float Light::GetRange () const
	{
		return m_range;
	}

	void Light::SetRange (float range)
	{
		m_range = range;
	}

	float Light::GetSpotAngle () const
	{
		return m_spotAngle;
	}

	void Light::SetSpotAngle (float angle)
	{
		m_spotAngle = angle;
	}

	EShadowType Light::GetShadowType () const
	{
		return m_shadowType;
	}

	void Light::SetShadowType (EShadowType shadowType)
	{
		m_shadowType = shadowType;
	}

	float Light::GetShadowIntensity () const
	{
		return m_shadowIntensity;
	}

	void Light::SetShadowIntensity (float shadowIntensity)
	{
		m_shadowIntensity = shadowIntensity;
	}

	float Light::GetShadowDepthBias () const
	{
		return m_shadowDepthBias;
	}

	void Light::SetShadowDepthBias (float shadowDepthBias)
	{
		m_shadowDepthBias = shadowDepthBias;
	}

	void Light::OnRenderEditor (Editor& editor)
	{
		editor.BeginComponent ("Light", this, true);

		int32 type = static_cast<int32> (m_type);
		editor.AddCombo ("Type", type, { "Directional", "Point", "Spot" });
		m_type = static_cast<ELightType> (type);

		editor.AddColorRGBA ("Color", m_color);

		editor.AddPropertyFloat ("Intensity", m_intensity);
		m_intensity = Math::Max (0.0f, m_intensity);

		if (m_type == ELightType::Point || m_type == ELightType::Spot)
		{
			editor.AddPropertyFloat ("Range", m_range);
			m_range = Math::Max (0.0f, m_range);
		}

		if (m_type == ELightType::Spot)
		{
			editor.AddPropertyFloat ("Spot Angle", m_spotAngle);
			m_spotAngle = Math::Clamp (m_spotAngle, 1.0f, 179.0f);
		}
	}

	void Light::OnSerialize (Json::Json& json) const
	{
		Component::OnSerialize (json);

		json["type"] = "Light";
		json["light type"] = static_cast<int32> (m_type);
		json["color"] = Json::JsonSerializer::Serialize<Vector4> (m_color);
		json["intensity"] = m_intensity;
		json["range"] = m_range;
		json["spot angle"] = m_spotAngle;
		json["shadow type"] = static_cast<int32> (m_shadowType);
		json["shadow intensity"] = m_shadowIntensity;
		json["shadow depth bias"] = m_shadowDepthBias;
	}

	void Light::OnDeserialize (const Json::Json& json)
	{
		int32 type;
		json.at ("light type").get_to (type);
		m_type = static_cast<ELightType> (type);

		m_color.m_x = json.at ("color")[0];
		m_color.m_y = json.at ("color")[1];
		m_color.m_z = json.at ("color")[2];
		m_color.m_w = json.at ("color")[3];

		json.at ("intensity").get_to (m_intensity);
		json.at ("range").get_to (m_range);
		json.at ("spot angle").get_to (m_spotAngle);

		int32 shadowtype;
		json.at ("light type").get_to (shadowtype);
		m_shadowType = static_cast<EShadowType> (shadowtype);

		json.at ("shadow intensity").get_to (m_shadowIntensity);
		json.at ("shadow depth bias").get_to (m_shadowDepthBias);
	}
}