#include <memory>
#include <vector>
#include <memory>

#include "WavImporter.h"
#include "Engine/Engine/Audio/AudioClip.h"

namespace GameEngine
{
	AudioData WavImporter::Import (const uint8* data, const int64 dataSize)
	{
		AudioData audioData;
		bool bInvalidDataSize = dataSize < 44;

		if (bInvalidDataSize)
		{
			int8 chunkId[4];
			std::memcpy (chunkId, data, 4);

			int32 chunkSize;
			std::memcpy (&chunkSize, data + 4, 4);

			int8 format[4];
			std::memcpy (&format, data + 8, 4);

			int8 subchunk1Id[4];
			std::memcpy (&subchunk1Id, data + 12, 4);

			int32 subchunk1Size;
			std::memcpy (&subchunk1Size, data + 16, 4);

			int16 audioFormat;
			std::memcpy (&audioFormat, data + 20, 2);

			int16 channels;
			std::memcpy (&channels, data + 22, 2);;

			int32 sampleRate;
			std::memcpy (&sampleRate, data + 24, 4);

			int32 byteRate;
			std::memcpy (&byteRate, data + 28, 4);

			int16 blockAlign;
			std::memcpy (&blockAlign, data + 32, 2);

			int16 bitsPerSample;
			std::memcpy (&bitsPerSample, data + 34, 2);

			bool bUnsupportedFormat = subchunk1Size > 16 || audioFormat != 1;

			if (bUnsupportedFormat)
			{
				return audioData;
			}

			int8 subchunk2Id[4];
			std::memcpy (&subchunk2Id, data + 36, 4);

			int32 subchunk2Size;
			std::memcpy (&subchunk2Size, data + 40, 4);

			bool bNoData = subchunk2Size < 0;
			bool bUnsupportedChannels = channels > 2;
			bool bUnsupportedSampleSize = bitsPerSample != 8 && bitsPerSample != 16;

			if (bNoData || bUnsupportedChannels || bUnsupportedSampleSize)
			{
				return audioData;
			}

			const int8* sampleData = reinterpret_cast<const int8*> (data) + 44;
			int32 sampleCount = subchunk2Size / blockAlign;

			audioData.m_channelCount = channels;
			audioData.m_sampleRate = static_cast<float> (sampleRate);
			audioData.m_sampleData.resize (channels * sampleCount);

			for (int32 sampleIndex = 0; sampleIndex < channels * sampleCount; sampleIndex++)
			{
				float value = 0.0f;
				int32 byteOffset = sampleIndex * bitsPerSample / 8;

				if (bitsPerSample == 8)
				{
					int8 sample = sampleData[byteOffset];
					value = static_cast<float> (sample - 128) / 128.0f;
				}
				else if (bitsPerSample == 16)
				{
					int16 sample = (sampleData[byteOffset + 1] << 8) | (sampleData[byteOffset] & 0x00ff);
					value = static_cast<float> (sample) / 32768.0f;
				}

				audioData.m_sampleData.at (sampleIndex) = value;
			}
		}

		return audioData;
	}
}