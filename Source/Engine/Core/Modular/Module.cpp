#include "Module.h"
#include "ModuleManager.h"

namespace GameEngine
{
	namespace Modular
	{
		ModuleBase::ModuleBase ()
		{
		}

		ModuleBase::~ModuleBase ()
		{
		}

		ModuleLink::ModuleLink (const std::string& moduleName, const PathString& modulePath) :
			m_moduleName (moduleName)
		{
			ModuleManager::RegisterModule (modulePath, this);
		}

		ModuleLink::~ModuleLink ()
		{
		}

		std::string ModuleLink::GetModuleName () const
		{
			return m_moduleName;
		}
	}
}