#include <utility>

#include "ModuleManager.h"

namespace GameEngine
{
	namespace Modular
	{
		ModuleManager::ModuleManager () :
			m_staticModuleLinkMap (),
			m_moduleInfoMap ()
		{
		}

		ModuleManager::~ModuleManager ()
		{
		}

		ModuleManager& ModuleManager::GetSingleton ()
		{
			static ModuleManager instance;

			return instance;
		}

		ModuleBase* ModuleManager::FindModule (const std::string& moduleName)
		{
			ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_moduleInfoMap;
			auto moduleInfoIter = moduleInfoMap.find (moduleName);

			if (moduleInfoIter == moduleInfoMap.end ())
			{
				return nullptr;
			}

			return moduleInfoIter->second->m_module.get ();
		}

		ModuleBase* ModuleManager::LoadModule (const std::string& moduleName)
		{
			ModuleBase* loadedModule = FindModule (moduleName);

			if (loadedModule != nullptr)
			{
				return loadedModule;
			}

			ModuleLinkMap& moduleLinkMap = ModuleManager::GetSingleton ().m_staticModuleLinkMap;
			auto moduleLinkIter = moduleLinkMap.find (moduleName);

			if (moduleLinkIter == moduleLinkMap.end ())
			{
				return nullptr;
			}

			ModuleLink* moduleLink = moduleLinkIter->second;
			ModuleHandle loadedModuleHandle = PlatformLoadModule (moduleLink->m_path);

			if (loadedModuleHandle == nullptr)
			{
				return nullptr;
			}

			std::string createFunctionName = "CreateModuleFunction" + moduleLink->m_name;
			ModuleCreateFunction createModuleFunction = PlatformFindModuleFunction (loadedModuleHandle, createFunctionName);

			if (createModuleFunction == nullptr)
			{
				return nullptr;
			}

			loadedModule = createModuleFunction ();

			auto moduleInfo = std::make_unique<ModuleInfo> ();
			moduleInfo->m_handle = loadedModuleHandle;
			moduleInfo->m_module = std::unique_ptr<ModuleBase> (loadedModule);

			ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_moduleInfoMap;
			moduleInfoMap.emplace (moduleName, std::move(moduleInfo));

			return loadedModule;
		}

		void ModuleManager::UnloadModule (const std::string& moduleName)
		{
			auto& moduleInfoMap = ModuleManager::GetSingleton ().m_moduleInfoMap;
			auto moduleInfoIter = moduleInfoMap.find (moduleName);

			if (moduleInfoIter == moduleInfoMap.end ())
			{
				return;
			}

			ModuleHandle moduleHandle = moduleInfoIter->second->m_handle;

			if (PlatformUnloadMoudule (moduleHandle))
			{
				moduleInfoMap.erase (moduleInfoIter);
			}	
		}

		void ModuleManager::RegisterModule (const std::string& moduleName, ModuleLink* moduleLink)
		{
			auto& moduleLinkMap = ModuleManager::GetSingleton ().m_staticModuleLinkMap;
			moduleLinkMap.emplace (moduleName, moduleLink);
		}
	}
}
