#include "Light.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (Light)

		Light::Light () : Component ("Light"),
		m_type (ELightType::Directional),
		m_color (Vector4::One),
		m_intensity (1.0f),
		m_range (0.0f),
		m_spotAngle (0.0f),
		m_shadowType (EShadowType::Hard),
		m_shadowIntensity (1.0f),
		m_shadowDepthBias (0.001f)
	{
	}

	Light::~Light ()
	{
	}

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

	void Light::OnRenderEditor ()
	{
		int32 type = EditorGUI::InputDropDown ("Type", static_cast<int32> (m_type), { "Directional", "Point", "Spot" });
		m_type = static_cast<ELightType> (type);

		m_color = EditorGUI::InputColorRGBA ("Color", m_color);
		m_intensity = Math::Max (0.0f, EditorGUI::InputFloat ("Intensity", m_intensity));

		if (m_type == ELightType::Point || m_type == ELightType::Spot)
		{
			m_range = Math::Max (0.0f, EditorGUI::InputFloat ("Range", m_range));
		}

		if (m_type == ELightType::Spot)
		{
			m_spotAngle = Math::Max (0.0f, EditorGUI::InputFloat ("Spot Angle", m_spotAngle));
		}

		int32 shadowType = static_cast<int32> (m_shadowType);
		m_shadowType = static_cast<EShadowType> (EditorGUI::InputDropDown ("Shadow Type", shadowType, { "None", "Hard", "Soft" }));
		m_shadowIntensity = Math::Clamp (EditorGUI::InputFloat ("Shadow Intensity", m_shadowIntensity), 0.0f, 1.0f);
		m_shadowDepthBias = Math::Max (EditorGUI::InputFloat ("Shadow Depth Bias", m_shadowDepthBias), 0.0f);
	}

	void Light::OnSerialize (Json::Json& json) const
	{
		Component::OnSerialize (json);

		Json::JsonSerializer::Serialize (json, "light type", static_cast<int32> (m_type));
		Json::JsonSerializer::Serialize (json, "color", m_color);
		Json::JsonSerializer::Serialize (json, "intensity", m_intensity);
		Json::JsonSerializer::Serialize (json, "range", m_range);
		Json::JsonSerializer::Serialize (json, "spot angle", m_spotAngle);
		Json::JsonSerializer::Serialize (json, "shadow type", static_cast<int32> (m_shadowType));
		Json::JsonSerializer::Serialize (json, "shadow intensity", m_shadowIntensity);
		Json::JsonSerializer::Serialize (json, "shadow depth bias", m_shadowDepthBias);
	}

	void Light::OnDeserialize (const Json::Json& json)
	{
		m_type = static_cast<ELightType> (Json::JsonSerializer::Deserialize<int32> (json, "light type"));
		m_color = Json::JsonSerializer::Deserialize<Vector4> (json, "color");
		m_intensity = Json::JsonSerializer::Deserialize<float> (json, "intensity");
		m_range = Json::JsonSerializer::Deserialize<float> (json, "range");
		m_spotAngle = Json::JsonSerializer::Deserialize<float> (json, "spot angle");
		m_shadowType = static_cast<EShadowType> (Json::JsonSerializer::Deserialize<int32> (json, "shadow type"));
		m_shadowIntensity = Json::JsonSerializer::Deserialize<float> (json, "shadow intensity");
		m_shadowDepthBias = Json::JsonSerializer::Deserialize<float> (json, "shadow depth bias");
	}
}