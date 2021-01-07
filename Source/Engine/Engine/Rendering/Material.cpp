#include <algorithm>

#include "Material.h"
#include "Engine/Platform/PlatformRenderingInterface.h"
#include "Engine/Core/Asset/AssetManager.h"
#include "Engine/Core/JSON/JsonSerializer.h"
#include "Editor/Core/EditorGUI.h"

namespace GameEngine
{
	REGISTER_OBJECT_CPP (Material)

	Shader* Material::GetShader () const
	{
		return m_shader;
	}

	void Material::SetShader (Shader* shader)
	{
		if (shader == m_shader)
		{
			return;
		}

		m_shader = shader;
		m_textureMap.clear ();
		m_parameterTypeMap.clear ();
		
		if (shader->IsShaderStageExist (EShaderStage::Vertex))
		{
			if (shader->IsShaderResourceExist (EShaderStage::Vertex, "$Globals"))
			{
				m_vertexShaderParameterData = std::make_unique<uint8[]> (shader->GetShaderResourceBinding (EShaderStage::Vertex, "$Globals").m_byteSize);
			}

			for (const auto& vsResource : shader->GetShaderResourceBindingMap (EShaderStage::Vertex))
			{
				if (vsResource.second.m_type == ShaderResourceBinding::Texture)
				{
					if (vsResource.first.find ("Sampler") != std::string::npos)
					{
						continue;
					}

					m_textureMap[vsResource.first] = nullptr;
				}
			}

			for (auto& parameter : shader->GetShaderParameterMap (EShaderStage::Vertex))
			{
				if (m_parameterTypeMap.find (parameter.first) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (parameter.first, "Float");
				}
			}
		}

		if (shader->IsShaderStageExist (EShaderStage::Pixel))
		{
			if (shader->IsShaderResourceExist (EShaderStage::Pixel, "$Globals"))
			{
				m_pixelShaderParameterData = std::make_unique<uint8[]> (shader->GetShaderResourceBinding (EShaderStage::Pixel, "$Globals").m_byteSize);
			}

			for (const auto& psResource : shader->GetShaderResourceBindingMap (EShaderStage::Pixel))
			{
				if (psResource.second.m_type == ShaderResourceBinding::Texture)
				{
					if (psResource.first.find ("Sampler") != std::string::npos)
					{
						continue;
					}

					m_textureMap[psResource.first] = nullptr;
				}
			}

			for (auto& parameter : shader->GetShaderParameterMap (EShaderStage::Pixel))
			{
				if (m_parameterTypeMap.find (parameter.first) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (parameter.first, "Float");
				}
			}
		}

		if (shader->IsShaderStageExist (EShaderStage::Hull))
		{
			if (shader->IsShaderResourceExist (EShaderStage::Hull, "$Globals"))
			{
				m_hullShaderParameterData = std::make_unique<uint8[]> (shader->GetShaderResourceBinding (EShaderStage::Hull, "$Globals").m_byteSize);
			}

			for (const auto& hsResource : shader->GetShaderResourceBindingMap (EShaderStage::Hull))
			{
				if (hsResource.second.m_type == ShaderResourceBinding::Texture)
				{
					if (hsResource.first.find ("Sampler") != std::string::npos)
					{
						continue;
					}

					m_textureMap[hsResource.first] = nullptr;
				}
			}

			for (auto& parameter : shader->GetShaderParameterMap (EShaderStage::Hull))
			{
				if (m_parameterTypeMap.find (parameter.first) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (parameter.first, "Float");
				}
			}
		}

		if (shader->IsShaderStageExist (EShaderStage::Domain))
		{
			if (shader->IsShaderResourceExist (EShaderStage::Domain, "$Globals"))
			{
				m_domainShaderParameterData = std::make_unique<uint8[]> (shader->GetShaderResourceBinding (EShaderStage::Domain, "$Globals").m_byteSize);
			}

			for (const auto& dsResource : shader->GetShaderResourceBindingMap (EShaderStage::Domain))
			{
				if (dsResource.second.m_type == ShaderResourceBinding::Texture)
				{
					if (dsResource.first.find ("Sampler") != std::string::npos)
					{
						continue;
					}

					m_textureMap[dsResource.first] = nullptr;
				}
			}

			for (auto& parameter : shader->GetShaderParameterMap (EShaderStage::Domain))
			{
				if (m_parameterTypeMap.find (parameter.first) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (parameter.first, "Float");
				}
			}
		}

		if (shader->IsShaderStageExist (EShaderStage::Geometry))
		{
			if (shader->IsShaderResourceExist (EShaderStage::Geometry, "$Globals"))
			{
				m_geometryShaderParameterData = std::make_unique<uint8[]> (shader->GetShaderResourceBinding (EShaderStage::Geometry, "$Globals").m_byteSize);
			}

			for (const auto& gsResource : shader->GetShaderResourceBindingMap (EShaderStage::Geometry))
			{
				if (gsResource.second.m_type == ShaderResourceBinding::Texture)
				{
					if (gsResource.first.find ("Sampler") != std::string::npos)
					{
						continue;
					}

					m_textureMap[gsResource.first] = nullptr;
				}
			}

			for (auto& parameter : shader->GetShaderParameterMap (EShaderStage::Geometry))
			{
				if (m_parameterTypeMap.find (parameter.first) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (parameter.first, "Float");
				}
			}
		}
	}

	int32 Material::GetInteger (const std::string& name) const
	{
		int32 value = 0;
		const uint8* dataStart = GetParameterDataStart (name);

		if (dataStart != nullptr)
		{
			value = *reinterpret_cast<const int32*> (dataStart);
		}

		return value;
	}

	void Material::SetInteger (const std::string& name, int32 value)
	{
		uint8* valueStart = reinterpret_cast<uint8*> (&value);
		uint8* valueEnd = valueStart + sizeof (int32);

		SetParameterData (name, valueStart, valueEnd);
	}

	float Material::GetFloat (const std::string& name) const
	{
		float value = 0.0f;
		const uint8* dataStart = GetParameterDataStart (name);

		if (dataStart != nullptr)
		{
			value = *reinterpret_cast<const float*> (dataStart);
		}

		return value;
	}

	void Material::SetFloat (const std::string& name, float value)
	{
		uint8* valueStart = reinterpret_cast<uint8*> (&value);
		uint8* valueEnd = valueStart + sizeof (float);

		SetParameterData (name, valueStart, valueEnd);
	}

	Vector4 Material::GetVector4 (const std::string& name) const
	{
		Vector4 value = Vector4::Zero;
		const uint8* dataStart = GetParameterDataStart (name);

		if (dataStart != nullptr)
		{
			value = *reinterpret_cast<const Vector4*> (dataStart);
		}

		return value;
	}

	void Material::SetVector4 (const std::string& name, Vector4 value)
	{
		uint8* valueStart = reinterpret_cast<uint8*> (&value);
		uint8* valueEnd = valueStart + sizeof (Vector4);

		SetParameterData (name, valueStart, valueEnd);
	}

	Matrix4x4 Material::GetMatrix4x4 (const std::string& name) const
	{
		Matrix4x4 value = Matrix4x4::Zero;
		const uint8* dataStart = GetParameterDataStart (name);

		if (dataStart != nullptr)
		{
			value = *reinterpret_cast<const Matrix4x4*> (dataStart);
		}

		return value;
	}

	void Material::SetMatrix4x4 (const std::string& name, Matrix4x4 value)
	{
		uint8* valueStart = reinterpret_cast<uint8*> (&value);
		uint8* valueEnd = valueStart + sizeof (Matrix4x4);

		SetParameterData (name, valueStart, valueEnd);
	}

	Texture* Material::GetTexture (const std::string& name) const
	{
		Texture* texture = nullptr;
		auto textureIter = m_textureMap.find (name);

		if (textureIter != m_textureMap.end ())
		{
			texture = textureIter->second;
		}

		return texture;
	}

	void Material::SetTexture (const std::string& name, Texture& texture)
	{
		if (m_textureMap.find (name) == m_textureMap.end ())
		{
			return;
		}

		m_textureMap.at (name) = &texture;
	}

	uint8* Material::GetVertexShaderParameterData () const
	{
		return m_vertexShaderParameterData.get ();
	}

	uint8* Material::GetPixelShaderParameterData () const
	{
		return m_pixelShaderParameterData.get ();
	}

	uint8* Material::GetHullShaderParameterData () const
	{
		return m_hullShaderParameterData.get ();
	}

	uint8* Material::GetDomainShaderParameterData () const
	{
		return m_domainShaderParameterData.get ();
	}

	uint8* Material::GetGeometryShaderParameterData () const
	{
		return m_geometryShaderParameterData.get ();
	}

	const std::unordered_map<std::string, Texture*>& Material::GetTextureMap () const
	{
		return m_textureMap;
	}

	void Material::OnRenderEditor ()
	{
		Shader* shader = static_cast<Shader*> (EditorGUI::InputAsset ("Shader", "Shader", m_shader));

		if (shader != nullptr)
		{
			SetShader (shader);

			for (auto& textureInfo : m_textureMap)
			{
				std::string textureName = textureInfo.first;
				const bool bGlobal = textureName.rfind ("g_", sizeof ("g_")) == 0;

				if (bGlobal)
				{
					continue;
				}

				Texture* texture = textureInfo.second;
				texture = static_cast<Texture*> (EditorGUI::InputAsset (textureName, "Texture", texture));

				if (texture != nullptr)
				{
					textureInfo.second = texture;
				}
			}

			for (auto& parameter : m_parameterTypeMap)
			{
				const std::string parameterName = parameter.first;
				const bool bGlobal = parameterName.rfind ("g_", sizeof ("g_")) == 0;

				if (bGlobal)
				{
					continue;
				}

				std::string typeName = m_parameterTypeMap[parameterName];
				int32 typeNumber = 0;

				if (typeName == "Vector4")
				{
					typeNumber = 1;
				}

				typeNumber = EditorGUI::InputDropDown ("##type-dropdown" + parameterName, typeNumber, { "Float", "Vector4" });
				EditorGUI::SameLine ();

				if (typeNumber == 0)
				{
					m_parameterTypeMap[parameterName] = "Float";
					float value = EditorGUI::InputFloat (parameterName, GetFloat (parameterName));
					SetFloat (parameterName, value);
				}
				else if (typeNumber == 1)
				{
					m_parameterTypeMap[parameterName] = "Vector4";
					Vector4 value = EditorGUI::InputVector4 (parameterName, GetVector4 (parameterName));
					SetVector4 (parameterName, value);
				}
			}
		}
	}

	void Material::OnSerialize (Json::Json& json) const
	{
		PathString shaderPath = AssetManager::GetInstance ().GetAssetPath (m_shader);
		Json::JsonSerializer::Serialize (json, "shader", shaderPath);
		Json::JsonSerializer::CreateArray (json, "textures");
		Json::JsonSerializer::CreateArray (json, "parameters");

		for (auto& pairNameTexture : m_textureMap)
		{
			std::string textureName = pairNameTexture.first;
			Texture* texture = pairNameTexture.second;

			if (texture == nullptr)
			{
				continue;
			}

			bool bGlobal = textureName.rfind ("g_", sizeof ("g_")) == 0;

			if (bGlobal)
			{
				continue;
			}

			Json::Json textureJson;
			Json::JsonSerializer::Serialize (textureJson, "name", textureName);
			Json::JsonSerializer::Serialize (textureJson, "path", AssetManager::GetInstance ().GetAssetPath (texture));
			Json::JsonSerializer::AppendArray (json, "textures", textureJson);
		}

		for (auto& pairNameType : m_parameterTypeMap)
		{
			std::string parameterName = pairNameType.first;
			std::string typeName = pairNameType.second;
			bool bGlobal = parameterName.rfind ("g_", sizeof ("g_")) == 0;

			if (bGlobal)
			{
				continue;
			}

			Json::Json parameterJson;
			Json::JsonSerializer::Serialize (parameterJson, "name", parameterName);
			Json::JsonSerializer::Serialize (parameterJson, "type", typeName);

			if (typeName == "Float")
			{
				Json::JsonSerializer::Serialize (parameterJson, "value", GetFloat (parameterName));
			}
			else if (typeName == "Vector4")
			{
				Json::JsonSerializer::Serialize (parameterJson, "value", GetVector4 (parameterName));
			}

			Json::JsonSerializer::AppendArray (json, "parameters", parameterJson);
		}
	}

	void Material::OnDeserialize (const Json::Json& json)
	{
		PathString shaderPath = Json::JsonSerializer::Deserialize<PathString> (json, "shader");
		auto* shader = AssetManager::GetInstance ().FindAsset<Shader> (shaderPath);
		SetShader (shader);

		for (auto it = Json::JsonSerializer::GetArrayBegin (json, "textures"); it != Json::JsonSerializer::GetArrayEnd (json, "textures"); ++it)
		{
			Json::Json textureJson = it.value ();
			std::string name = Json::JsonSerializer::Deserialize<std::string> (textureJson, "name");
			PathString texturePath = Json::JsonSerializer::Deserialize<PathString> (textureJson, "path");
			Texture* texture = AssetManager::GetInstance ().FindAsset<Texture> (texturePath);

			if (texture != nullptr)
			{
				m_textureMap.emplace (name, texture);
			}
		}

		for (auto it = Json::JsonSerializer::GetArrayBegin (json, "parameters"); it != Json::JsonSerializer::GetArrayEnd (json, "parameters"); ++it)
		{
			Json::Json parameterJson = it.value ();
			std::string name = Json::JsonSerializer::Deserialize<std::string> (parameterJson, "name");
			std::string type = Json::JsonSerializer::Deserialize<std::string> (parameterJson, "type");

			m_parameterTypeMap.emplace (name, type);

			if (type == "Float")
			{
				float value = Json::JsonSerializer::Deserialize<float> (parameterJson, "value");
				SetFloat (name, value);
			}
			else if (type == "Vector4")
			{
				Vector4 value = Json::JsonSerializer::Deserialize<Vector4> (parameterJson, "value");
				SetVector4 (name, value);
			}
		}
	}

	const uint8* Material::GetParameterDataStart (const std::string& name) const
	{
		uint8* dataStart = nullptr;

		if (m_shader->IsShaderParameterExist (EShaderStage::Vertex, name))
		{
			uint32 byteOffset = m_shader->GetShaderParameter (EShaderStage::Vertex, name).m_byteOffset;
			dataStart = m_vertexShaderParameterData.get () + byteOffset;
		}
		else if (m_shader->IsShaderParameterExist (EShaderStage::Pixel, name))
		{
			uint32 byteOffset = m_shader->GetShaderParameter (EShaderStage::Pixel, name).m_byteOffset;
			dataStart = m_pixelShaderParameterData.get () + byteOffset;
		}
		else if (m_shader->IsShaderParameterExist (EShaderStage::Hull, name))
		{
			uint32 byteOffset = m_shader->GetShaderParameter (EShaderStage::Hull, name).m_byteOffset;
			dataStart = m_hullShaderParameterData.get () + byteOffset;
		}
		else if (m_shader->IsShaderParameterExist (EShaderStage::Domain, name))
		{
			uint32 byteOffset = m_shader->GetShaderParameter (EShaderStage::Domain, name).m_byteOffset;
			dataStart = m_domainShaderParameterData.get () + byteOffset;
		}
		else if (m_shader->IsShaderParameterExist (EShaderStage::Geometry, name))
		{
			uint32 byteOffset = m_shader->GetShaderParameter (EShaderStage::Geometry, name).m_byteOffset;
			dataStart = m_geometryShaderParameterData.get () + byteOffset;
		}

		return dataStart;
	}

	void Material::SetParameterData (const std::string& name, const uint8* dataStart, const uint8* dataEnd)
	{
		if (m_shader == nullptr)
		{
			return;
		}

		uint8* parameterData;
		uint32 parameterByteOffset;

		if (m_shader->IsShaderParameterExist (EShaderStage::Vertex, name))
		{
			parameterData = m_vertexShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Vertex, name).m_byteOffset;
			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Pixel, name))
		{
			parameterData = m_pixelShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Pixel, name).m_byteOffset;
			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Hull, name))
		{
			parameterData = m_hullShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Hull, name).m_byteOffset;
			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Domain, name))
		{
			parameterData = m_domainShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Domain, name).m_byteOffset;
			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Geometry, name))
		{
			parameterData = m_geometryShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Geometry, name).m_byteOffset;
			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
		}
	}
}