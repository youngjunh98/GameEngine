#pragma once

#include "Type.h"
#include "Engine/Core/CoreMacro.h"
#include "Engine/RI/RenderingInterfaceEnum.h"

namespace GameEngine
{
	struct ENGINE_CORE_API StbImage
	{
		StbImage ();
		~StbImage ();

		bool IsSquare () const;
		bool IsPowerOfTwo () const;

		bool IsGray () const;
		bool IsAlphaChannel () const;

		ERenderingResourceFormat GetFormat () const;
		int32 GetRowPitch () const;

		// Pointer to Image Data
		void* m_data;

		// Width of Image
		int32 m_width;

		// Height of Image
		int32 m_height;

		// Number of Channel
		// 1 -> Gray
		// 2 -> Gray + Alpha
		// 3 -> RGB
		// 4 -> RGB + Alpha
		int32 m_channels;

		// Byte Size of Pixel Component
		// 1 -> 8-bit
		// 2 -> 16-bit
		// 4 -> 32-bit
		int32 m_byte;

		// Is Image Color Space Linear?
		bool m_bLinear;
	};
}
