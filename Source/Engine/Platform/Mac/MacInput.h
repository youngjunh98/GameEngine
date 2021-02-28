#pragma once

#include "Engine/Platform/Generic/GenericInput.h"

namespace GameEngine
{
	namespace Platform
	{
		class ENGINE_PLATFORM_API MacInput : public GenericInput
		{
		public:
			MacInput () {}
			virtual ~MacInput () {}

			virtual bool Initialize () override;
			virtual void Shutdown () override;
		};
	}
}
