#include <cstring>

#include "FBXMemoryReadStream.h"
#include "Engine/Core/Math/Math.h"

namespace GameEngine
{
	FBXMemoryReadStream::FBXMemoryReadStream (int64 size, fbxsdk::FbxManager* fbxManager) :
		m_streamData (nullptr),
		m_streamDataSize (size),
		m_state (EState::eEmpty),
		m_streamPosition (0),
		m_readerId (fbxManager->GetIOPluginRegistry ()->FindReaderIDByExtension ("fbx"))
	{
	}

	FBXMemoryReadStream::~FBXMemoryReadStream ()
	{
	}

	fbxsdk::FbxStream::EState FBXMemoryReadStream::GetState ()
	{
		return m_state;
	}

	bool FBXMemoryReadStream::Open (void* pStreamData)
	{
		m_streamData = static_cast<int8*> (pStreamData);
		m_streamPosition = 0;
		m_state = EState::eOpen;

		return true;
	}

	bool FBXMemoryReadStream::Close ()
	{
		m_streamData = nullptr;
		m_streamPosition = 0;
		m_state = EState::eClosed;

		return true;
	}

	bool FBXMemoryReadStream::Flush ()
	{
		return true;
	}

	int FBXMemoryReadStream::Write (const void* pData, int pSize)
	{
		return 0;
	}

	int FBXMemoryReadStream::Read (void* pData, int pSize) const
	{
		if (m_streamData == nullptr)
		{
			return 0;
		}

		int64 maxReadSize = m_streamDataSize - m_streamPosition;
		int64 readSize = Math::Min (maxReadSize, static_cast<int64> (pSize));

		std::memcpy (pData, &m_streamData[m_streamPosition], readSize);
		const_cast<FBXMemoryReadStream*> (this)->m_streamPosition += readSize;

		return static_cast<int> (readSize);
	}

	int FBXMemoryReadStream::GetReaderID () const
	{
		return m_readerId;
	}

	int FBXMemoryReadStream::GetWriterID () const
	{
		return -1;
	}

	void FBXMemoryReadStream::Seek (const fbxsdk::FbxInt64& pOffset, const fbxsdk::FbxFile::ESeekPos& pSeekPos)
	{
		int64 offset = static_cast<int64> (pOffset);

		if (pSeekPos == FbxFile::ESeekPos::eBegin)
		{
			m_streamPosition = offset;
		}
		else if (pSeekPos == FbxFile::ESeekPos::eEnd)
		{
			m_streamPosition = m_streamDataSize - offset;
		}
		else
		{
			m_streamPosition += offset;
		}
	}

	long FBXMemoryReadStream::GetPosition () const
	{
		return static_cast<long> (m_streamPosition);
	}

	void FBXMemoryReadStream::SetPosition (long pPosition)
	{
		m_streamPosition = static_cast<int64> (pPosition);
	}

	int FBXMemoryReadStream::GetError () const
	{
		return 0;
	}

	void FBXMemoryReadStream::ClearError ()
	{
		// Empty
	}
}