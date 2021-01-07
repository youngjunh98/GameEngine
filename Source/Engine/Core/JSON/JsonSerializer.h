#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Type.h"
#include "Json.h"
#include "Engine/Core/CoreMacro.h"
#include "Engine/Core/Object/Object.h"
#include "Engine/Core/Object/ObjectFactory.h"
#include "Engine/Core/Math/Math.h"

namespace GameEngine
{
	namespace Json
	{
		class ENGINE_CORE_API JsonSerializer final
		{
		private:
			JsonSerializer ();
			virtual ~JsonSerializer () = 0;

			static bool IsArray (const Json& json, const std::string& name);

		public:
			static void CreateArray (Json& json, const std::string& name);
			static void AppendArray (Json& json, const std::string& name, const Json& jsonToAppend);
			static JsonIterator GetArrayBegin (Json& json, const std::string& name);
			static JsonIterator GetArrayEnd (Json& json, const std::string& name);
			static JsonConstIterator GetArrayBegin (const Json& json, const std::string& name);
			static JsonConstIterator GetArrayEnd (const Json& json, const std::string& name);

			static Json ObjectToJson (const Object& object);
			static std::shared_ptr<Object> JsonToObject (const Json& json);
			static void SerializeObject (Json& json, const std::string& name, const Object& object);
			static std::shared_ptr<Object> DeserializeObject (const Json& json, const std::string& name);

			template<typename T>
			static void Serialize (Json& json, const std::string& name, const T& value)
			{
				json[name] = value;
			}

			template<typename T>
			static T Deserialize (const Json& json, const std::string& name)
			{
				T value = {};

				if (json.contains (name))
				{
					value = json[name].get<T> ();
				}

				return value;
			}

			template<>
			static void Serialize<Vector2> (Json& json, const std::string& name, const Vector2& value)
			{
				json[name] = Json::array ();
				json[name].push_back (value.m_x);
				json[name].push_back (value.m_y);
			}

			template<>
			static Vector2 Deserialize<Vector2> (const Json& json, const std::string& name)
			{
				Vector2 value;

				if (IsArray (json, name))
				{
					value.m_x = json[name][0].get<float> ();
					value.m_y = json[name][1].get<float> ();
				}

				return value;
			}

			template<>
			static void Serialize<Vector3> (Json& json, const std::string& name, const Vector3& value)
			{
				json[name] = Json::array ();
				json[name].push_back (value.m_x);
				json[name].push_back (value.m_y);
				json[name].push_back (value.m_z);
			}

			template<>
			static Vector3 Deserialize<Vector3> (const Json& json, const std::string& name)
			{
				Vector3 value;

				if (IsArray (json, name))
				{
					value.m_x = json[name][0].get<float> ();
					value.m_y = json[name][1].get<float> ();
					value.m_z = json[name][2].get<float> ();
				}

				return value;
			}

			template<>
			static void Serialize<Vector4> (Json& json, const std::string& name, const Vector4& value)
			{
				json[name] = Json::array ();
				json[name].push_back (value.m_x);
				json[name].push_back (value.m_y);
				json[name].push_back (value.m_z);
				json[name].push_back (value.m_w);
			}

			template<>
			static Vector4 Deserialize<Vector4> (const Json& json, const std::string& name)
			{
				Vector4 value;

				if (IsArray (json, name))
				{
					value.m_x = json[name][0].get<float> ();
					value.m_y = json[name][1].get<float> ();
					value.m_z = json[name][2].get<float> ();
					value.m_w = json[name][3].get<float> ();
				}

				return value;
			}

			template<>
			static void Serialize<Quaternion> (Json& json, const std::string& name, const Quaternion& value)
			{
				json[name] = Json::array ();
				json[name].push_back (value.m_x);
				json[name].push_back (value.m_y);
				json[name].push_back (value.m_z);
				json[name].push_back (value.m_w);
			}

			template<>
			static Quaternion Deserialize<Quaternion> (const Json& json, const std::string& name)
			{
				Quaternion value;

				if (IsArray (json, name))
				{
					value.m_x = json[name][0].get<float> ();
					value.m_y = json[name][1].get<float> ();
					value.m_z = json[name][2].get<float> ();
					value.m_w = json[name][3].get<float> ();
				}

				return value;
			}
		};
	}
}