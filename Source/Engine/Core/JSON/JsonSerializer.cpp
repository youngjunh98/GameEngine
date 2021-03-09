#include "Engine/Core/JSON/JsonSerializer.h"

namespace GameEngine
{
	namespace Json
	{
		bool JsonSerializer::IsArray (const Json& json, const std::string& name)
		{
			if (json.contains (name))
			{
				if (json[name].is_array ())
				{
					return true;
				}
			}

			return false;
		}

		void JsonSerializer::CreateArray (Json& json, const std::string& name)
		{
			json[name] = Json::array ();
		}

		void JsonSerializer::AppendArray (Json& json, const std::string& name, const Json& jsonToAppend)
		{
			if (IsArray (json, name))
			{
				json[name].push_back (jsonToAppend);
			}
		}

		JsonIterator JsonSerializer::GetArrayBegin (Json& json, const std::string& name)
		{
			JsonIterator iterator = json.end ();

			if (IsArray (json, name))
			{
				iterator = json[name].begin ();
			}

			return iterator;
		}

		JsonIterator JsonSerializer::GetArrayEnd (Json& json, const std::string& name)
		{
			JsonIterator iterator = json.end ();

			if (IsArray (json, name))
			{
				iterator = json[name].end ();
			}

			return iterator;
		}

		JsonConstIterator JsonSerializer::GetArrayBegin (const Json& json, const std::string& name)
		{
			JsonConstIterator iterator = json.end ();

			if (IsArray (json, name))
			{
				iterator = json[name].begin ();
			}

			return iterator;
		}

		JsonConstIterator JsonSerializer::GetArrayEnd (const Json& json, const std::string& name)
		{
			JsonConstIterator iterator = json.end ();

			if (IsArray (json, name))
			{
				iterator = json[name].end ();
			}

			return iterator;
		}

		Json JsonSerializer::ObjectToJson (const Object& object)
		{
			Json json;
			json["type"] = object.GetTypeName ();
			object.OnSerialize (json);

			return json;
		}

		std::shared_ptr<Object> JsonSerializer::JsonToObject (const Json& json)
		{
			std::shared_ptr<Object> object;

			if (json.contains ("type"))
			{
				std::string type = Deserialize<std::string> (json, "type");
				ObjectFactoryFunction function = ObjectFactory::GetFunction (type);
				object = function ();

				if (object != nullptr)
				{
					object->OnDeserialize (json);
				}
			}

			return object;
		}

		void JsonSerializer::SerializeObject (Json& json, const std::string& name, const Object& object)
		{
			json[name] = ObjectToJson (object);
		}

		std::shared_ptr<Object> JsonSerializer::DeserializeObject (const Json& json, const std::string& name)
		{
			std::shared_ptr<Object> instance;

			if (json.contains (name))
			{
				instance = JsonToObject (json[name]);
			}

			return instance;
		}
	}
}
