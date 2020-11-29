#ifndef INCLUDE_OBJECT
#define INCLUDE_OBJECT

#include <string>

#include "Editor/Editor.h"
#include "Core/JSON/JsonSerializer.h"

namespace GameEngine
{
	class Object
	{
	public:
		Object (const std::string& name = "");
		virtual ~Object () = 0;

		virtual void Destroy ();

		std::string GetName () const;
		void SetName (const std::string& name);

		bool IsAlive () const;

		virtual void OnRenderEditor (Editor& editor) {}
		virtual void OnSerialize (Json::Json& json) const;
		virtual void OnDeserialize (const Json::Json& json);

	private:
		bool m_bAlive;
		std::string m_name;
	};
}

#endif