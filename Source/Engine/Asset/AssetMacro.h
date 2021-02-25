#include "Engine/Core/Modular/ModuleMacro.h"

#if defined(EXPORT_ENGINE_ASSET)
	#define ENGINE_ASSET_API EXPORT_MODULE
#elif defined(IMPORT_ENGINE_ASSET)
	#define ENGINE_ASSET_API IMPORT_MODULE
#else
	#define ENGINE_ASSET_API
#endif
