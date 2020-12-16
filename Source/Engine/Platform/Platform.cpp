#include "Engine/Platform/Platform.h"

namespace GameEngine
{
	namespace Platform
	{
		GenericApplication& GetGenericApplication ()
		{
			static Application platformApplicationSingleton;
			return platformApplicationSingleton;
		}

		GenericInput& GetGenericInput ()
		{
			static Input platformInputSingleton;
			return platformInputSingleton;
		}

		GenericTimer& GetGenericTimer ()
		{
			static Timer platformTimerSingleton;
			return platformTimerSingleton;
		}
	}
}