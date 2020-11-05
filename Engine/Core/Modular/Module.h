#pragma once

#include <string>

#include "Core/CoreMacro.h"

namespace GameEngine
{
	namespace Modular
	{
		using ModuleHandle = void*;

		class CORE_API ModuleBase
		{
		public:
			ModuleBase ();
			virtual ~ModuleBase () = 0;
		};

		class CORE_API ModuleLink
		{
		public:
			ModuleLink (const std::string& name, const std::string& path);
			~ModuleLink ();

			std::string m_name;
			std::string m_path;
		};
	}
}

#define LINK_MODULE(NAME, PATH, MODULE_CLASS)\
extern "C" EXPORT_MODULE ::GameEngine::Modular::ModuleBase* CreateModuleFunction##NAME ()\
{\
	return new MODULE_CLASS ();\
}\
\
GameEngine::Modular::ModuleLink ModuleLink##NAME (#NAME, #PATH);
