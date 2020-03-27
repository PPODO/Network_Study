#pragma once
#include <Windows.h>

class CCriticalSection {
private:
	CRITICAL_SECTION m_CriticalSection;

public:
	CCriticalSection() {
		InitializeCriticalSection(&m_CriticalSection);\
	}

	~CCriticalSection() {
		DeleteCriticalSection(&m_CriticalSection);
	}

public:
	void Lock() {
		EnterCriticalSection(&m_CriticalSection);
	}

	void UnLock() {
		LeaveCriticalSection(&m_CriticalSection);
	}

};