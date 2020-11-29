#pragma once

#include <string>

#include "Core/CoreMacro.h"
#include "Core/File/FileSystem.h"

namespace GameEngine
{
	namespace Modular
	{
		using ModuleHandle = void*;

		class ModuleBase
		{
		public:
			CORE_API ModuleBase ();
			CORE_API virtual ~ModuleBase () = 0;
		};

		class ModuleLink final
		{
		public:
			CORE_API ModuleLink (const std::string& moduleName, const PathString& modulePath);
			CORE_API ~ModuleLink ();

			CORE_API std::string GetModuleName () const;

		private:
			std::string m_moduleName;
		};
	}
}

#define LINK_MODULE(MODULE_NAME, MODULE_PATH, MODULE_CLASS)\
extern "C" EXPORT_MODULE ::GameEngine::Modular::ModuleBase* CreateModuleFunction##MODULE_NAME ()\
{\
	return new MODULE_CLASS ();\
}\
\
::GameEngine::Modular::ModuleLink ModuleLink##NAME (#MODULE_NAME, PATH(#MODULE_PATH));
