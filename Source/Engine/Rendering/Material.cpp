#include <algorithm>

#include "Material.h"
#include "Platform/PlatformRenderingInterface.h"
#include "Engine/Rendering/GlobalRenderer.h"
#include "Engine/Asset/AssetManager.h"


namespace GameEngine
{
	Shader* Material::GetShader () const
	{
		return m_shader;
	}

	void Material::SetShader (Shader* shader)
	{
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

	int32 Material::GetInteger (const std::string& name)
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

	float Material::GetFloat (const std::string& name)
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

	Vector4 Material::GetVector4 (const std::string& name)
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

	Matrix4x4 Material::GetMatrix4x4 (const std::string& name)
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

	Texture* Material::GetTexture (const std::string& name)
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

	const std::unordered_map<std::string, Texture*>& Material::GetTextureMap () const
	{
		return m_textureMap;
	}

	void Material::OnRenderEditor (Editor& editor)
	{
		std::wstring assetPath = g_assetManager.GetAssetPath (m_shader);
		editor.AddPropertyAsset ("Shader", "Shader", assetPath);
		auto* foundShader = g_assetManager.LoadShader (assetPath);

		if (foundShader != m_shader)
		{
			SetShader (foundShader);
		}

		if (m_shader != nullptr)
		{
			for (auto& texture : m_textureMap)
			{
				assetPath = g_assetManager.GetAssetPath (texture.second);
				editor.AddPropertyAsset (texture.first, "Texture", assetPath);
				texture.second = g_assetManager.FindAsset<Texture> (assetPath);
			}

			for (auto& parameter : m_parameterTypeMap)
			{
				if (m_parameterTypeMap.find (parameter.first) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (parameter.first, "Float");
				}

				if (m_parameterTypeMap[parameter.first] == "Float")
				{
					float value = GetFloat (parameter.first);
					editor.AddPropertyShaderParameter (parameter.first, m_parameterTypeMap[parameter.first], &value);
					SetFloat (parameter.first, value);
				}
				else if (m_parameterTypeMap[parameter.first] == "Vector")
				{
					Vector4 value = GetVector4 (parameter.first);
					editor.AddPropertyShaderParameter (parameter.first, m_parameterTypeMap[parameter.first], &value);
					SetVector4 (parameter.first, value);
				}
			}
		}
	}

	void Material::OnSerialize (Json::Json& json) const
	{
		auto shaderPath = g_assetManager.GetAssetPath (m_shader);
		std::string asciiShaderPath (shaderPath.begin (), shaderPath.end ());

		json["shader"] = asciiShaderPath;
		json["textures"] = Json::Json::array ();
		json["parameters"] = Json::Json::array ();

		for (auto& texture : m_textureMap)
		{
			if (texture.second == nullptr)
			{
				continue;
			}

			auto texturePath = g_assetManager.GetAssetPath (texture.second);
			std::string asciiTexturePath (texturePath.begin (), texturePath.end ());

			Json::Json textureInfo;
			textureInfo["name"] = texture.first;
			textureInfo["path"] = asciiTexturePath;

			json["textures"].push_back (textureInfo);
		}

		for (auto& parameter : m_parameterTypeMap)
		{
			Json::Json parameterInfo;
			parameterInfo["name"] = parameter.first;
			parameterInfo["type"] = parameter.second;

			json["parameters"].push_back (parameterInfo);
		}
	}

	void Material::OnDeserialize (const Json::Json& json)
	{
		std::string shaderPath;

		if (json.find ("shader") != json.end ())
		{
			if (json.at ("shader").is_string ())
			{
				json.at ("shader").get_to (shaderPath);

				std::wstring unicodePath (shaderPath.begin (), shaderPath.end ());
				auto* shader = g_assetManager.LoadShader (unicodePath);

				SetShader (shader);
			}
		}

		if (json.find ("textures") != json.end ())
		{
			for (auto& textureInfo : json["textures"].items ())
			{
				std::string name;
				std::string texturePath;

				textureInfo.value ().at ("name").get_to (name);
				textureInfo.value ().at ("path").get_to (texturePath);

				std::wstring unicodeTexturePath (texturePath.begin (), texturePath.end ());
				Texture* texture = g_assetManager.FindAsset<Texture> (unicodeTexturePath);

				if (texture == nullptr)
				{
					continue;
				}

				if (m_textureMap.find (name) == m_textureMap.end ())
				{
					m_textureMap.emplace (name, texture);
				}
				else
				{
					m_textureMap[name] = texture;
				}
			}
		}


		if (json.find ("parameters") != json.end ())
		{
			for (auto& textureInfo : json["parameters"].items ())
			{
				std::string name;
				std::string type;

				textureInfo.value ().at ("name").get_to (name);
				textureInfo.value ().at ("type").get_to (type);

				if (m_parameterTypeMap.find (name) == m_parameterTypeMap.end ())
				{
					m_parameterTypeMap.emplace (name, type);
				}
				else
				{
					m_parameterTypeMap[name] = type;
				}
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
		RI_ShaderConstantBuffer* parameterBuffer;
		uint8* parameterData;
		uint32 parameterByteOffset;

		if (m_shader->IsShaderParameterExist (EShaderStage::Vertex, name))
		{
			parameterBuffer = m_shader->GetVertexShaderParameterBuffer ();
			parameterData = m_vertexShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Vertex, name).m_byteOffset;

			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
			g_renderer.GetRenderingInterface ().UpdateShaderConstantBuffer (parameterBuffer, parameterData);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Pixel, name))
		{
			parameterBuffer = m_shader->GetPixelShaderParameterBuffer ();
			parameterData = m_pixelShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Pixel, name).m_byteOffset;

			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
			g_renderer.GetRenderingInterface ().UpdateShaderConstantBuffer (parameterBuffer, parameterData);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Hull, name))
		{
			parameterBuffer = m_shader->GetHullShaderParameterBuffer ();
			parameterData = m_hullShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Hull, name).m_byteOffset;

			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
			g_renderer.GetRenderingInterface ().UpdateShaderConstantBuffer (parameterBuffer, parameterData);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Domain, name))
		{
			parameterBuffer = m_shader->GetDomainShaderParameterBuffer ();
			parameterData = m_domainShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Domain, name).m_byteOffset;

			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
			g_renderer.GetRenderingInterface ().UpdateShaderConstantBuffer (parameterBuffer, parameterData);
		}

		if (m_shader->IsShaderParameterExist (EShaderStage::Geometry, name))
		{
			parameterBuffer = m_shader->GetGeometryShaderParameterBuffer ();
			parameterData = m_geometryShaderParameterData.get ();
			parameterByteOffset = m_shader->GetShaderParameter (EShaderStage::Geometry, name).m_byteOffset;

			std::copy (dataStart, dataEnd, parameterData + parameterByteOffset);
			g_renderer.GetRenderingInterface ().UpdateShaderConstantBuffer (parameterBuffer, parameterData);
		}
	}
}