#pragma once

#include <string>
#include <memory>

#include "ObjectFactory.h"
#include "Engine/Core/CoreMacro.h"
#include "Engine/Core/JSON/JsonSerializer.h"

#define REGISTER_OBJECT_HEADER(OBJECT_CLASS_NAME)\
virtual ::std::string GetTypeName () const override { return #OBJECT_CLASS_NAME; }

#define REGISTER_OBJECT_CPP(OBJECT_CLASS_NAME)\
REGISTER_OBJECT_FACTORY (OBJECT_CLASS_NAME)\

namespace GameEngine
{
	class ENGINE_CORE_API Object
	{
	public:
		Object (const std::string& name = "");
		virtual ~Object () = 0;

		virtual void Destroy ();

		std::string GetName () const;
		void SetName (const std::string& name);

		bool IsAlive () const;

		virtual std::string GetTypeName () const = 0;

		static std::shared_ptr<Object> InstantiateByTypeName (const std::string& typeName);

		virtual void OnRenderEditor () {}
		virtual void OnSerialize (Json::Json& json) const;
		virtual void OnDeserialize (const Json::Json& json);

	private:
		bool m_bAlive;
		std::string m_name;
	};
}
