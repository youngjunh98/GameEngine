#include "Engine/Core/Modular/ModuleMacro.h"

#if defined(EXPORT_ENGINE)
	#define ENGINE_API EXPORT_MODULE
#elif defined(IMPORT_ENGINE)
	#define ENGINE_API IMPORT_MODULE
#else
	#define ENGINE_API
#endif