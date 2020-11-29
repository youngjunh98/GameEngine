#pragma once

#include "Core/Modular/ModuleMacro.h"

#if defined(EXPORT_RI)
	#define RI_API EXPORT_MODULE
#elif defined(IMPORT_RI)
	#define RI_API IMPORT_MODULE
#else
	#define RI_API
#endif