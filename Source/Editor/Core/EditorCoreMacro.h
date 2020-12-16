#include "Engine/Core/Modular/ModuleMacro.h"

#if defined(EXPORT_EDITOR_CORE)
	#define EDITOR_CORE_API EXPORT_MODULE
#elif defined(IMPORT_EDITOR_CORE)
	#define EDITOR_CORE_API IMPORT_MODULE
#else
	#define EDITOR_CORE_API
#endif