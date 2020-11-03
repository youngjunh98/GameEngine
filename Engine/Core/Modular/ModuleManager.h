#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "ModuleBase.h"
#include "Core/CoreMacro.h"

namespace GameEngine
{
	namespace Modular
	{
		class ModuleLink;

		using ModulePointer = void*;
		using ModuleCreateFunction = ModuleBase* (*) ();

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
			using ModuleMap = std::unordered_map<std::string, std::unique_ptr<ModuleBase>>;

			ModuleLinkMap m_staticModuleLinkMap;
			ModuleMap m_moduleMap;
		};

		class ModuleLink
		{
		public:
			ModuleLink (const std::string& name, const std::string& path) :
				m_name (name),
				m_path (path)
			{
				auto& manager = ModuleManager::GetSingleton ();
				manager.RegisterModule (name, this);
			}

			~ModuleLink ()
			{}

			std::string m_name;
			std::string m_path;
		};

		ModulePointer PlatformImportModule (const std::string& path);
		ModuleCreateFunction PlatformFindModuleFunction (void* moduleHandle, const std::string& functionName);
	}
}

#define LINK_MODULE(NAME, PATH, MODULE_CLASS)\
extern "C" EXPORT_MODULE ::GameEngine::Modular::ModuleBase* CreateModuleFunction##NAME ()\
{\
	return new MODULE_CLASS ();\
}\
\
GameEngine::Modular::ModuleLink ModuleLink##NAME (#NAME, #PATH);

