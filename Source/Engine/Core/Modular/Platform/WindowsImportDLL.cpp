#include <Windows.h>
#include <string>

#include "Engine/Core/Modular/ModuleManager.h"

namespace GameEngine
{
	namespace Modular
	{
		ModuleHandle PlatformLoadModule (const PathString& path)
		{
			HINSTANCE hModule = LoadLibrary (path.c_str ());

			if (hModule == NULL)
			{
				return nullptr;
			}

			return hModule;
		}

		bool PlatformUnloadMoudule (ModuleHandle moduleHandle)
		{
			bool bSucceed = false;

			if (moduleHandle != nullptr)
			{
				HMODULE hModule = reinterpret_cast<HMODULE> (moduleHandle);
				bSucceed = FreeLibrary (hModule) != 0;
			}

			return bSucceed;
		}

		ModuleCreateFunction PlatformFindModuleFunction (ModuleHandle moduleHandle, const std::string& functionName)
		{
			ModuleCreateFunction result = nullptr;

			if (moduleHandle != nullptr)
			{
				auto* functionAddress = GetProcAddress (reinterpret_cast<HMODULE> (moduleHandle), functionName.c_str ());
				result = reinterpret_cast<ModuleCreateFunction> (functionAddress);
			}

			return result;
		}
	}
}