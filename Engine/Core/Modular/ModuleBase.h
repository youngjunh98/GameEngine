#pragma once

#define EXPORT_MODULE __declspec(dllexport)
#define IMPORT_MODULE __declspec(dllimport)

namespace GameEngine
{
	namespace Modular
	{
		class ModuleBase
		{
		public:
			ModuleBase () {}
			virtual ~ModuleBase () = 0 {}
		};
	}
}