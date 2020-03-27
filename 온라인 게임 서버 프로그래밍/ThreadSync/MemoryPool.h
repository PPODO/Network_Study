#pragma once
#include <cstdlib>
#include "ThreadSync.h"

template<typename T, size_t ALLOC_BLOCK_COUNT = 50>
class CMemoryPool : public CMultiThreadSync {
private:
	static UCHAR* m_FreePointer;

private:
	static void AllocBlock() {
		m_FreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_COUNT];

		UCHAR** Current = reinterpret_cast<UCHAR**>(m_FreePointer);
		UCHAR* Next = m_FreePointer;

		for (size_t i = 0; i < ALLOC_BLOCK_COUNT; i++) {
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<UCHAR**>(Next);
		}
		*Current = nullptr;
	}

public:
	static void* operator new(std::size_t AllocLength) {
		if (!m_FreePointer) {
			AllocBlock();
		}
		UCHAR* ReturnPointer = m_FreePointer;
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		return ReturnPointer;
	}

	static void operator delete(void* DeletePointer) {
		*reinterpret_cast<UCHAR**>(DeletePointer) = m_FreePointer;
		m_FreePointer = static_cast<UCHAR*>(DeletePointer);
	}

}; 

template<typename T, size_t ALLOC_BLOCK_COUNT>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_COUNT>::m_FreePointer;