#include "ModuleManager.h"

namespace GameEngine
{
	namespace Modular
	{
		ModuleManager::ModuleManager () :
			m_staticModuleLinkMap (),
			m_moduleMap ()
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
			auto& moduleMap = ModuleManager::GetSingleton ().m_moduleMap;
			auto moduleIter = moduleMap.find (moduleName);

			if (moduleIter == moduleMap.end ())
			{
				return nullptr;
			}

			return moduleIter->second.get ();
		}

		ModuleBase* ModuleManager::LoadModule (const std::string& moduleName)
		{
			auto* loadedModule = FindModule (moduleName);

			if (loadedModule != nullptr)
			{
				return loadedModule;
			}

			auto& moduleLinkMap = ModuleManager::GetSingleton ().m_staticModuleLinkMap;
			auto moduleLinkIter = moduleLinkMap.find (moduleName);

			if (moduleLinkIter == moduleLinkMap.end ())
			{
				return nullptr;
			}

			auto* moduleLink = moduleLinkIter->second;
			ModulePointer importedModule = PlatformImportModule (moduleLink->m_path);

			if (importedModule == nullptr)
			{
				return nullptr;
			}

			ModuleCreateFunction createModuleFunction = PlatformFindModuleFunction (importedModule, "CreateModuleFunction" + moduleLink->m_name);

			if (createModuleFunction == nullptr)
			{
				return nullptr;
			}

			loadedModule = createModuleFunction ();

			auto& moduleMap = ModuleManager::GetSingleton ().m_moduleMap;
			moduleMap.emplace (moduleName, loadedModule);

			return loadedModule;
		}

		void ModuleManager::UnloadModule (const std::string& moduleName)
		{
			auto& moduleMap = ModuleManager::GetSingleton ().m_moduleMap;
			auto moduleIter = moduleMap.find (moduleName);

			if (moduleIter == moduleMap.end ())
			{
				return;
			}

			moduleMap.erase (moduleIter);
		}

		void ModuleManager::RegisterModule (const std::string& moduleName, ModuleLink* moduleLink)
		{
			auto& moduleLinkMap = ModuleManager::GetSingleton ().m_staticModuleLinkMap;
			moduleLinkMap.emplace (moduleName, moduleLink);
		}
	}
}
