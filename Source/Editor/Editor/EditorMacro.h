#include "Engine/Core/Modular/ModuleMacro.h"

#if defined(EXPORT_EDITOR)
	#define EDITOR_API EXPORT_MODULE
#elif defined(IMPORT_EDITOR)
	#define EDITOR_API IMPORT_MODULE
#else
	#define EDITOR_API
#endif