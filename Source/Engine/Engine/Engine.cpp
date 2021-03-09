#include "Engine.h"
#include "Engine/Platform/Platform.h"
#include "Engine/Core/Modular/ModuleManager.h"
#include "Engine/Asset/AssetManager.h"
#include "Engine/Asset/AssetImporter.h"
#include "Engine/Engine/Audio/Audio.h"
#include "Engine/Engine/Rendering/GlobalRenderer.h"
#include "Engine/Engine/Scene/SceneManager.h"
#include "Engine/Engine/Physics/Physics.h"

namespace GameEngine
{
	Engine::Engine ()
	{
	}

	Engine::~Engine ()
	{
	}

	Engine& Engine::GetInstance ()
	{
		static Engine instance;
		return instance;
	}

	bool Engine::Init ()
	{
		Modular::ModuleManager::Initialize ();

		GlobalRendererSettings rendererSettings = { 800, 800, false, 4, true, 60, true };

		if (Platform::GetGenericApplication ().Initialize (PATH("게임 엔진"), 800, 800, false) == false)
		{
			return false;
		}

		if (Platform::GetGenericInput ().Initialize () == false)
		{
			return false;
		}

		if (GlobalRenderer::Init (rendererSettings) == false)
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

		return true;
	}

	void Engine::Shutdown ()
	{
		m_postRenderEvents.clear ();

		Audio::Shutdown ();

		SceneManager::GetInstance ().UnloadScene ();
		AssetManager::UnloadAllAssets ();

		g_physics.Shutdown ();
		GlobalRenderer::Shutdown ();

		Platform::GetGenericInput ().Shutdown ();
		Platform::GetGenericApplication ().Shutdown ();
	}

	void Engine::Run ()
	{
		auto& platformApplication = Platform::GetGenericApplication ();
		auto& platformInput = Platform::GetGenericInput ();
		auto& platformTimer = Platform::GetGenericTimer ();

		AssetManager::LoadAllAssets (PATH ("Assets"));
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
					SceneManager::GetInstance ().FixedUpdateScene ();
					g_physics.Simulate (fixedDeltaTime);

					accumulatedTime -= fixedDeltaTime;
				}

				platformTimer.SetAccumulatedTime (accumulatedTime);
			}

			SceneManager::GetInstance ().UpdateScene ();
			GlobalRenderer::ClearRenderTargetAndDepthStencil (Vector4 (0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);
			GlobalRenderer::RenderScene (SceneManager::GetInstance ().GetScene ());

			for (auto postRenderEvent : m_postRenderEvents)
			{
				postRenderEvent ();
			}

			GlobalRenderer::PresentSwapChain ();

			platformInput.Update ();
		}
	}

	void Engine::AddPostRenderCallback (PostRenderEvent callback)
	{
		m_postRenderEvents.push_back (callback);
	}

	void Engine::RemovePostRenderCallback (PostRenderEvent callback)
	{
		for (auto it = m_postRenderEvents.begin (); it != m_postRenderEvents.end (); it++)
		{
			if (*it == callback)
			{
				m_postRenderEvents.erase (it);
				break;
			}
		}
	}
}
