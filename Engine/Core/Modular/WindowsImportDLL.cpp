#include <Windows.h>
#include <string>

#include "Core/Modular/ModuleManager.h"

namespace GameEngine
{
	namespace Modular
	{
		ModulePointer PlatformImportModule (const std::string& path)
		{
			std::wstring wPath (path.begin (), path.end ());
			HINSTANCE hInstanceDLL = LoadLibrary (wPath.c_str ());

			if (hInstanceDLL == NULL)
			{
				return nullptr;
			}

			return hInstanceDLL;
		}

		ModuleCreateFunction PlatformFindModuleFunction (void* moduleHandle, const std::string& functionName)
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