#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Type.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Engine/EngineMacro.h"
#include "Engine/Engine/Rendering/Shader.h"
#include "Engine/Engine/Rendering/Texture2D.h"

namespace GameEngine
{
	class ENGINE_API Material : public Object
	{
	public:
		REGISTER_OBJECT_HEADER (Material)

		Material () : Object (), m_shader (nullptr) {}
		virtual ~Material () {}

		Shader* GetShader () const;
		void SetShader (Shader* shader);

		int32 GetInteger (const std::string& name) const;
		void SetInteger (const std::string& name, int32 value);

		float GetFloat (const std::string& name) const;
		void SetFloat (const std::string& name, float value);

		Vector4 GetVector4 (const std::string& name) const;
		void SetVector4 (const std::string& name, Vector4 value);

		Matrix4x4 GetMatrix4x4 (const std::string& name) const;
		void SetMatrix4x4 (const std::string& name, Matrix4x4 value);

		Texture* GetTexture (const std::string& name) const;
		void SetTexture (const std::string& name, Texture& texture);

		uint8* GetVertexShaderParameterData () const;
		uint8* GetPixelShaderParameterData () const;
		uint8* GetHullShaderParameterData () const;
		uint8* GetDomainShaderParameterData () const;
		uint8* GetGeometryShaderParameterData () const;

		const std::unordered_map<std::string, Texture*>& GetTextureMap () const;

		virtual void OnRenderEditor () override;
		virtual void OnSerialize (Json::Json& json) const override;
		virtual void OnDeserialize (const Json::Json& json) override;

	private:
		const uint8* GetParameterDataStart (const std::string& name) const;
		void SetParameterData (const std::string& name, const uint8* dataStart, const uint8* dataEnd);

	private:
		Shader* m_shader;
		std::unique_ptr<uint8[]> m_vertexShaderParameterData;
		std::unique_ptr<uint8[]> m_pixelShaderParameterData;
		std::unique_ptr<uint8[]> m_hullShaderParameterData;
		std::unique_ptr<uint8[]> m_domainShaderParameterData;
		std::unique_ptr<uint8[]> m_geometryShaderParameterData;
		std::unordered_map<std::string, Texture*> m_textureMap;
		std::unordered_map<std::string, std::string> m_parameterTypeMap;
	};
}
