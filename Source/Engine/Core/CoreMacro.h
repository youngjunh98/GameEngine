#include "Modular/ModuleMacro.h"

#if defined(EXPORT_ENGINE_CORE)
	#define ENGINE_CORE_API EXPORT_MODULE
#elif defined(IMPORT_ENGINE_CORE)
	#define ENGINE_CORE_API IMPORT_MODULE
#else
	#define ENGINE_CORE_API
#endif
