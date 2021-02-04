#pragma once

namespace GameEngine
{
	struct StbImageData
	{
		StbImageData ();
		~StbImageData ();

		// Pointer to Image Data
		void* m_data;
	};
}
