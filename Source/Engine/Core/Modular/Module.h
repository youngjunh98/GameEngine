#pragma once

#include <string>

#include "Engine/Core/CoreMacro.h"
#include "Engine/Core/File/FileSystem.h"

namespace GameEngine
{
	namespace Modular
	{
		using ModuleHandle = void*;

		class ModuleBase
		{
		public:
			ENGINE_CORE_API ModuleBase ();
			ENGINE_CORE_API virtual ~ModuleBase () = 0;
		};

		class ModuleLink final
		{
		public:
			ENGINE_CORE_API ModuleLink (const std::string& moduleName, const PathString& modulePath);
			ENGINE_CORE_API ~ModuleLink ();

			ENGINE_CORE_API std::string GetModuleName () const;

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
