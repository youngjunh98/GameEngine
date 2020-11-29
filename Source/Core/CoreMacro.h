#include "Modular/ModuleMacro.h"

#if defined(EXPORT_CORE)
	#define CORE_API EXPORT_MODULE
#elif defined(IMPORT_CORE)
	#define CORE_API IMPORT_MODULE
#else
	#define CORE_API
#endif
