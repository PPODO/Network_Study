#pragma once
#include "CriticalSection.h"
#include <iostream>

class CMultiThreadSync {
	friend class CThreadSync;
private:
	CCriticalSection m_Sync;

public:
	class CThreadSync {
	private:
		CMultiThreadSync* const m_ThreadSyncPtr;
	public:
		CThreadSync(CMultiThreadSync* ThreadSyncPtr) : m_ThreadSyncPtr(ThreadSyncPtr) {
			if (m_ThreadSyncPtr) {
				m_ThreadSyncPtr->m_Sync.Lock();
			}
		}
		
		~CThreadSync() {
			if (m_ThreadSyncPtr) {
				m_ThreadSyncPtr->m_Sync.UnLock();
			}
		}
	};
};