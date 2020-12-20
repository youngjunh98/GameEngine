#pragma once

#if defined (snprintf)
	#undef snprintf
#endif

#include <memory>
#include <string>
#include <single_include/nlohmann/json.hpp>

#include "Type.h"
#include "Engine/Core/Object/ObjectFactory.h"
#include "Engine/Core/Math/Math.h"

namespace GameEngine
{
	namespace Json
	{
		using Json = nlohmann::json;

		class JsonSerializer
		{
		public:
			JsonSerializer ();
			virtual ~JsonSerializer () = 0;

			template<typename TObject>
			static Json SerializeObject (const TObject& object)
			{
				static_assert (IsBaseOf<Object, TObject>::Result, "TObject is not derived from GameEngine::Object!");

				Json json;
				json["type"] = object.GetTypeName ();
				object.OnSerialize (json);

				return json;
			}

			template<typename TObject>
			static std::shared_ptr<TObject> DeserializeObject (const Json& json)
			{
				static_assert (IsBaseOf<Object, TObject>::Result, "TObject is not derived from GameEngine::Object!");

				if (json.find ("type") == json.end ())
				{
					return nullptr;
				}

				std::string type = json["type"].get<std::string> ();

				ObjectFactoryFunction function = ObjectFactory::GetFunction (type);
				std::shared_ptr<TObject> instance = std::dynamic_pointer_cast<TObject> (function ());

				if (instance != nullptr)
				{
					instance->OnDeserialize (json);
				}

				return instance;
			}

			template<typename T>
			static void Serialize (Json& json, const std::string& name, const T& value);

			template<typename T>
			static T Deserialize (const Json& json, const std::string& name);

			template<>
			static void Serialize<int32> (Json& json, const std::string& name, const int32& value)
			{
				json[name] = value;
			}

			template<>
			static int32 Deserialize<int32> (const Json& json, const std::string& name)
			{
				int32 value = 0;

				if (json.contains (name))
				{
					value = json[name].get<int32> ();
				}

				return value;
			}

			template<>
			static void Serialize<float> (Json& json, const std::string& name, const float& value)
			{
				json[name] = value;
			}

			template<>
			static float Deserialize<float> (const Json& json, const std::string& name)
			{
				float value = 0.0f;

				if (json.contains (name))
				{
					value = json[name].get<float> ();
				}

				return value;
			}

			template<>
			static void Serialize<bool> (Json& json, const std::string& name, const bool& value)
			{
				json[name] = value;
			}

			template<>
			static bool Deserialize<bool> (const Json& json, const std::string& name)
			{
				bool value = false;

				if (json.contains (name))
				{
					value = json[name].get<bool> ();
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

				if (json.contains (name))
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

				if (json.contains (name))
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

				if (json.contains (name))
				{
					value.m_x = json[name][0].get<float> ();
					value.m_y = json[name][1].get<float> ();
					value.m_z = json[name][2].get<float> ();
					value.m_w = json[name][2].get<float> ();
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

				if (json.contains (name))
				{
					value.m_x = json[name][0].get<float> ();
					value.m_y = json[name][1].get<float> ();
					value.m_z = json[name][2].get<float> ();
					value.m_w = json[name][2].get<float> ();
				}

				return value;
			}

			template<>
			static void Serialize<std::string> (Json& json, const std::string& name, const std::string& value)
			{
				json[name] = value;
			}

			template<>
			static std::string Deserialize<std::string> (const Json& json, const std::string& name)
			{
				std::string value;

				if (json.contains (name))
				{
					value = json[name].get<std::string> ();
				}

				return value;
			}

			template<>
			static void Serialize<std::wstring> (Json& json, const std::string& name, const std::wstring& value)
			{
				json[name] = value;
			}

			template<>
			static std::wstring Deserialize<std::wstring> (const Json& json, const std::string& name)
			{
				std::wstring value;

				if (json.contains (name))
				{
					value = json[name].get<std::wstring> ();
				}

				return value;
			}
		};
	}
}