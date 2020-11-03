#pragma once

#include "Core/Modular/ModuleBase.h"

#if defined(EXPORT_PLATFORM)
	#define PLATFORM_API EXPORT_MODULE
#elif defined(IMPORT_PLATFORM)
	#define PLATFORM_API IMPORT_MODULE
#else
	#define PLATFORM_API
#endif