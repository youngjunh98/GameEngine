#include "Object.h"

namespace GameEngine
{
	Object::Object (const std::string& name) :
		m_bAlive (true),
		m_name (name)
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

	void Object::OnSerialize (Json::Json& json) const
	{
		json["type"] = "object";
		json["name"] = m_name;
	}

	void Object::OnDeserialize (const Json::Json& json)
	{
		json.at ("name").get_to (m_name);
	}
}
