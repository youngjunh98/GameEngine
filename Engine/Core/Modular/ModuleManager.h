#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Module.h"
#include "Core/CoreMacro.h"

namespace GameEngine
{
	namespace Modular
	{
		using ModuleCreateFunction = ModuleBase * (*) ();

		ModuleHandle PlatformLoadModule (const std::string& path);
		bool PlatformUnloadMoudule (ModuleHandle moduleHandle);
		ModuleCreateFunction PlatformFindModuleFunction (ModuleHandle moduleHandle, const std::string& functionName);

		class ModuleInfo
		{
		public:
			ModuleInfo () : m_module (nullptr), m_handle (nullptr) {}
			~ModuleInfo () {}

			std::unique_ptr<ModuleBase> m_module;
			ModuleHandle m_handle;
		};

		class ModuleManager
		{
		public:
			CORE_API static ModuleManager& GetSingleton ();

			CORE_API static ModuleBase* FindModule (const std::string& moduleName);
			CORE_API static ModuleBase* LoadModule (const std::string& moduleName);
			CORE_API static void UnloadModule (const std::string& moduleName);

			CORE_API static void RegisterModule (const std::string& moduleName, ModuleLink* moduleLink);

		private:
			ModuleManager ();
			~ModuleManager ();

		private:
			using ModuleLinkMap = std::unordered_map<std::string, ModuleLink*>;
			using ModuleInfoMap = std::unordered_map<std::string, std::unique_ptr<ModuleInfo>>;

			ModuleLinkMap m_staticModuleLinkMap;
			ModuleInfoMap m_moduleInfoMap;
		};
	}
}
