#pragma once

#if defined (snprintf)
	#undef snprintf
#endif

#include <single_include/nlohmann/json.hpp>

#include "EngineType.h"
#include "Core/Math/Math.h"

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

			template<typename ObjectType>
			static Json Serialize (const ObjectType& object)
			{
				Json json;
				object.OnSerialize (json);

				return json;
			}

			template<>
			static Json Serialize<Vector2> (const Vector2& value)
			{
				Json json = Json::array ();
				json.push_back (value.m_x);
				json.push_back (value.m_y);

				return json;
			}

			template<>
			static Json Serialize<Vector3> (const Vector3& value)
			{
				Json json = Json::array ();
				json.push_back (value.m_x);
				json.push_back (value.m_y);
				json.push_back (value.m_z);

				return json;
			}

			template<>
			static Json Serialize<Vector4> (const Vector4& value)
			{
				Json json = Json::array ();
				json.push_back (value.m_x);
				json.push_back (value.m_y);
				json.push_back (value.m_z);
				json.push_back (value.m_w);

				return json;
			}

			template<>
			static Json Serialize<Quaternion> (const Quaternion& value)
			{
				Json json = Json::array ();
				json.push_back (value.m_x);
				json.push_back (value.m_y);
				json.push_back (value.m_z);
				json.push_back (value.m_w);

				return json;
			}

			//template<typename ObjectType>
			//static void Deserialize (ObjectType& object, const Json& json)
			//{
			//	object.OnDeserialize (json);
			//}


			//template<>
			//static void Deserialize<Vector2> (Vector2& value, const Json& json)
			//{
			//	value.m_x = json[0];
			//	value.m_y = json[1];
			//}

			//template<>
			//static void Deserialize<Vector3> (Vector3& value, const Json& json)
			//{
			//	value.m_x = json[0];
			//	value.m_y = json[1];
			//	value.m_z = json[2];
			//}

			//template<>
			//static void Deserialize<Vector4> (Vector4& value, const Json& json)
			//{
			//	value.m_x = json[0];
			//	value.m_y = json[1];
			//	value.m_z = json[2];
			//	value.m_w = json[3];
			//}

			//template<>
			//static void Deserialize<Quaternion> (Quaternion& value, const Json& json)
			//{
			//	value.m_x = json[0];
			//	value.m_y = json[1];
			//	value.m_z = json[2];
			//	value.m_w = json[3];
			//}
		};
	}
}