#include "RenderingInterface.h"
#include "Engine/Core/Modular/ModuleManager.h"

namespace GameEngine
{
	PathString RenderingInterface::ModulePath;
	RenderingInterface* RenderingInterface::ModuleInstance = nullptr;

	RenderingInterface::RenderingInterface ()
	{
	}

	RenderingInterface::~RenderingInterface ()
	{
	}

	bool RenderingInterface::LoadModule (ERenderingInterfaceModule renderingInterfaceModule)
	{
		bool bSucceed = false;
		
		if (ModuleInstance == nullptr)
		{
			switch (renderingInterfaceModule)
			{
			case ERenderingInterfaceModule::D3D11:
				ModulePath = Modular::ModuleManager::GetModulePath ("D3D11RI");
				ModuleInstance = static_cast<RenderingInterface*> (Modular::ModuleManager::CreateModuleInstance (ModulePath));
				break;
			}

			bSucceed = ModuleInstance != nullptr;
		}

		return bSucceed;
	}

	void RenderingInterface::UnloadModule ()
	{
		if (ModuleInstance == nullptr)
		{
			return;
		}

		ModuleInstance->Shutdown ();
		ModuleInstance = nullptr;

		Modular::ModuleManager::UnloadModule (ModulePath);
	}

	RenderingInterface& RenderingInterface::GetModule ()
	{
		return *ModuleInstance;
	}

	void RenderingInterface::Shutdown ()
	{
		m_resourceRefList.clear ();
	}
}
