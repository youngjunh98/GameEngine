#include "Engine.h"
#include "Platform/Platform.h"
#include "Core/Modular/ModuleManager.h"
#include "Audio/Audio.h"
#include "Asset/AssetImporter.h"
#include "Editor/Editor.h"
#include "Rendering/GlobalRenderer.h"
#include "Scene/SceneManager.h"
#include "Asset/AssetManager.h"
#include "Physics/Physics.h"

namespace GameEngine
{
	Engine g_engine;
	Editor g_editor;

	bool Engine::Init ()
	{
		Modular::ModuleManager::Initialize ();

		GlobalRendererSettings rendererSettings = { 800, 800, false, 4, true, 60, true };

		if (Platform::GetGenericApplication ().Initialize (L"게임 엔진", 800, 800, false) == false)
		{
			return false;
		}

		if (Platform::GetGenericInput ().Initialize () == false)
		{
			return false;
		}

		if (g_renderer.Init (rendererSettings) == false)
		{
			return false;
		}

		if (g_physics.Init () == false)
		{
			return false;
		}

		if (Audio::Initialize () == false)
		{
			return false;
		}

		g_editor.Initialize ();

		return true;
	}

	void Engine::Shutdown ()
	{
		g_editor.Shutdown ();

		Audio::Shutdown ();

		g_sceneManager.UnloadScene ();
		g_assetManager.Shutdown ();
		
		g_physics.Shutdown ();
		g_renderer.Shutdown ();

		Platform::GetGenericInput ().Shutdown ();
		Platform::GetGenericApplication ().Shutdown ();
	}

	void Engine::Run ()
	{
		auto& platformApplication = Platform::GetGenericApplication ();
		auto& platformInput = Platform::GetGenericInput ();
		auto& platformTimer = Platform::GetGenericTimer ();

		AssetImporter::ImportAllAssets ();
		platformTimer.Reset (1.0f / 60.0f);

		while (platformApplication.Update ())
		{
			platformTimer.Tick ();

			float deltaTime = platformTimer.GetDeltaTime ();

			if (deltaTime > 0.0f)
			{
				float accumulatedTime = platformTimer.GetAccumulatedTime () + deltaTime;
				float fixedDeltaTime = platformTimer.GetFixedDeltaTime ();

				while (accumulatedTime >= fixedDeltaTime)
				{
					g_sceneManager.FixedUpdateScene ();
					g_physics.Simulate (fixedDeltaTime);

					accumulatedTime -= fixedDeltaTime;
				}

				platformTimer.SetAccumulatedTime (accumulatedTime);
			}

			g_sceneManager.UpdateScene ();
			g_renderer.RenderScene (g_sceneManager.GetScene ());
			g_editor.RenderGUI ();
			g_renderer.PresentScreen ();

			platformInput.Update ();
		}	
	}
}