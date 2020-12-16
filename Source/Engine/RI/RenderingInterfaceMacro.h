#pragma once

#include "Engine/Core/Modular/ModuleMacro.h"

#if defined(EXPORT_ENGINE_RI)
	#define ENGINE_RI_API EXPORT_MODULE
#elif defined(IMPORT_ENGINE_RI)
	#define ENGINE_RI_API IMPORT_MODULE
#else
	#define ENGINE_RI_API
#endif