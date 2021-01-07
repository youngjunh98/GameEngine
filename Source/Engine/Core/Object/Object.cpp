#include "Object.h"
#include "Engine/Core/JSON/JsonSerializer.h"

namespace GameEngine
{
	Object::Object (const std::string& name) :
		m_bAlive (true), m_name (name)
	{
	}

	Object::~Object ()
	{
	}

	void Object::Destroy ()
	{
		m_bAlive = false;
		m_name.clear ();
	}

	std::string Object::GetName () const
	{
		return m_name;
	}

	void Object::SetName (const std::string& name)
	{
		m_name = name;
	}

	bool Object::IsAlive () const
	{
		return m_bAlive;
	}

	std::shared_ptr<Object> Object::InstantiateByTypeName (const std::string& typeName)
	{
		ObjectFactoryFunction factoryFunction = ObjectFactory::GetFunction (typeName);
		std::shared_ptr<Object> instance = factoryFunction ();
		return instance;
	}

	void Object::OnSerialize (Json::Json& json) const
	{
		Json::JsonSerializer::Serialize (json, "name", m_name);
	}

	void Object::OnDeserialize (const Json::Json& json)
	{
		m_name = Json::JsonSerializer::Deserialize<std::string> (json, "name");
	}
}
