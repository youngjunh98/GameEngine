#include "Platform/Generic/GenericApplication.h"

namespace GameEngine
{
	namespace Platform
	{
		GenericApplication::GenericApplication ()
		{
		}

		GenericApplication::~GenericApplication ()
		{
		}

		void GenericApplication::AddResizeListener (ApplicationResizeCallback callback)
		{
			m_resizeListeners.push_back (callback);
		}

		void GenericApplication::RemoveResizeListener (ApplicationResizeCallback callback)
		{
			for (auto callbackIterator = m_resizeListeners.begin (); callbackIterator != m_resizeListeners.end (); )
			{
				if (*callbackIterator == callback)
				{
					callbackIterator = m_resizeListeners.erase (callbackIterator);
				}
				else
				{
					++callbackIterator;
				}
			}
		}

		void GenericApplication::ExecuteResizeCallbacks (uint32 width, uint32 height)
		{
			for (auto& callback : m_resizeListeners)
			{
				callback (width, height);
			}
		}
	}
}
