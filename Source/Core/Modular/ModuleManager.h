#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "Module.h"
#include "Core/CoreMacro.h"
#include "Core/File/FileSystem.h"

namespace GameEngine
{
	namespace Modular
	{
		using ModuleCreateFunction = ModuleBase* (*) ();

		ModuleHandle PlatformLoadModule (const PathString& path);
		bool PlatformUnloadMoudule (ModuleHandle moduleHandle);
		ModuleCreateFunction PlatformFindModuleFunction (ModuleHandle moduleHandle, const std::string& functionName);

		class ModuleInfo
		{
		public:
			ModuleInfo () : m_instance (nullptr), m_handle (nullptr) {}
			~ModuleInfo () {}

			std::unique_ptr<ModuleBase> m_instance;
			ModuleHandle m_handle;
		};

		class ModuleManager final
		{
		public:
			CORE_API static void Initialize ();

			CORE_API static void RegisterModule (const PathString& modulePath, ModuleLink* moduleLink);
			CORE_API static bool LoadModule (const PathString& modulePath);
			CORE_API static void UnloadModule (const PathString& modulePath);

			CORE_API static ModuleBase* FindModuleInstance (const PathString& modulePath);
			CORE_API static ModuleBase* CreateModuleInstance (const PathString& modulePath);
			CORE_API static void DestroyModuleInstance (const PathString& modulePath);

			CORE_API static PathString GetModulePath (const std::string& moduleName);

		private:
			ModuleManager ();
			~ModuleManager ();

			static ModuleManager& GetSingleton ();

		private:
			using ModuleLinkMap = std::unordered_map<PathString, ModuleLink*>;
			using ModuleInfoMap = std::unordered_map<PathString, std::unique_ptr<ModuleInfo>>;

			ModuleLinkMap m_linkMap;
			ModuleInfoMap m_infoMap;
		};
	}
}
