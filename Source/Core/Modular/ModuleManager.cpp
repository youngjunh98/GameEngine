#include <utility>

#include "ModuleManager.h"
#include "Platform/Platform.h"

namespace GameEngine
{
	namespace Modular
	{
		ModuleManager::ModuleManager () :
			m_linkMap (), m_infoMap ()
		{
		}

		ModuleManager::~ModuleManager ()
		{
		}

		void ModuleManager::Initialize ()
		{
			PathString applicationPath = Platform::GetGenericApplication ().GetPath ();
			PathString moduleSerachPath = FileSystem::AddDirectorySeparator (FileSystem::RemoveFileName (applicationPath));

			for (PathString fileName : FileSystem::GetFileList (moduleSerachPath))
			{
				PathString extension = FileSystem::GetFileExtension (fileName);

				if (extension != PATH ("dll"))
				{
					continue;
				}

				LoadModule (fileName);
			}
		}

		void ModuleManager::RegisterModule (const PathString& modulePath, ModuleLink* moduleLink)
		{
			auto& moduleLinkMap = ModuleManager::GetSingleton ().m_linkMap;
			moduleLinkMap.emplace (modulePath, moduleLink);
		}

		bool ModuleManager::LoadModule (const PathString& modulePath)
		{
			ModuleHandle moduleHandle = PlatformLoadModule (modulePath);

			if (moduleHandle == nullptr)
			{
				return false;
			}

			auto moduleInfo = std::make_unique<ModuleInfo> ();
			moduleInfo->m_handle = moduleHandle;

			ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_infoMap;
			moduleInfoMap.emplace (modulePath, std::move (moduleInfo));

			return true;
		}

		void ModuleManager::UnloadModule (const const PathString& modulePath)
		{
			DestroyModuleInstance (modulePath);

			ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_infoMap;
			auto moduleInfoIter = moduleInfoMap.find (modulePath);

			if (moduleInfoIter == moduleInfoMap.end ())
			{
				return;
			}

			ModuleInfo& moduleInfo = *moduleInfoIter->second;

			if (PlatformUnloadMoudule (moduleInfo.m_handle))
			{
				moduleInfo.m_handle = nullptr;
			}
		}

		ModuleBase* ModuleManager::FindModuleInstance (const PathString& modulePath)
		{
			ModuleBase* found = nullptr;
			ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_infoMap;
			auto moduleInfoIter = moduleInfoMap.find (modulePath);

			if (moduleInfoIter != moduleInfoMap.end ())
			{
				found = moduleInfoIter->second->m_instance.get ();
			}

			return found;
		}

		ModuleBase* ModuleManager::CreateModuleInstance (const PathString& modulePath)
		{
			ModuleBase* moduleInstance = FindModuleInstance (modulePath);
			bool bModuleInstanceNotFound = moduleInstance == nullptr;

			if (bModuleInstanceNotFound)
			{
				ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_infoMap;
				auto moduleInfoIter = moduleInfoMap.find (modulePath);

				ModuleLinkMap& moduleLinkMap = ModuleManager::GetSingleton ().m_linkMap;
				auto moduleLinkIter = moduleLinkMap.find (modulePath);

				if (moduleInfoIter != moduleInfoMap.end () && moduleLinkIter != moduleLinkMap.end ())
				{
					ModuleInfo& moduleInfo = *moduleInfoIter->second;
					ModuleHandle moduleHandle = moduleInfo.m_handle;

					ModuleLink& moduleLink = *moduleLinkIter->second;
					std::string moduleName = moduleLink.GetModuleName ();

					ModuleCreateFunction createModuleFunction = PlatformFindModuleFunction (moduleHandle, "CreateModuleFunction" + moduleName);

					if (createModuleFunction != nullptr)
					{
						moduleInstance = createModuleFunction ();

						if (moduleInstance != nullptr)
						{
							moduleInfo.m_instance = std::unique_ptr<ModuleBase> (moduleInstance);
						}
					}
				}
			}

			return moduleInstance;
		}

		void ModuleManager::DestroyModuleInstance (const PathString& modulePath)
		{
			ModuleInfoMap& moduleInfoMap = ModuleManager::GetSingleton ().m_infoMap;
			auto moduleInfoIter = moduleInfoMap.find (modulePath);

			if (moduleInfoIter == moduleInfoMap.end ())
			{
				return;
			}

			ModuleInfo& moduleInfo = *moduleInfoIter->second;
			moduleInfo.m_instance = nullptr;
		}

		PathString ModuleManager::GetModulePath (const std::string& moduleName)
		{
			PathString modulePath = PATH ("");

			for (const auto& pairPathLink : ModuleManager::GetSingleton ().m_linkMap)
			{
				if (pairPathLink.second->GetModuleName () == moduleName)
				{
					modulePath = pairPathLink.first;
					break;
				}
			}

			return modulePath;
		}

		ModuleManager& ModuleManager::GetSingleton ()
		{
			static ModuleManager instance;
			return instance;
		}
	}
}
