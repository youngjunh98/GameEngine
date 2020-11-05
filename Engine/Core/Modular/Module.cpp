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

        ModuleLink::ModuleLink (const std::string& name, const std::string& path) :
            m_name (name), m_path (path)
        {
            auto& manager = ModuleManager::GetSingleton ();
            manager.RegisterModule (name, this);
        }

        ModuleLink::~ModuleLink ()
        {}
    }
}