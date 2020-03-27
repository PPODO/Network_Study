#pragma once
#include "ThreadSync.h"

static const size_t MAX_QUEUE_LENGTH = 250;

template<typename T>
class CCircularQueue : public CMultiThreadSync {
private:
	T m_Queue[MAX_QUEUE_LENGTH];

private:
	size_t m_Head, m_Tail;
	size_t m_CurrentDataCount;

public:
	CCircularQueue() : m_Head(0), m_Tail(0), m_CurrentDataCount(0) {};

public:
	bool Push(const T& Data) {
		CThreadSync Sync(this);
		size_t TempTail = (m_Tail + 1) % MAX_QUEUE_LENGTH;
		if (TempTail == m_Head) {
			return false;
		}
		m_Tail = TempTail;
		m_Queue[TempTail] = Data;
		m_CurrentDataCount++;
		return true;
	}
	
	bool Pop(T& Data) {
		CThreadSync Sync(this);
		size_t TempHead = (m_Head + 1) % MAX_QUEUE_LENGTH;
		if (TempHead == m_Tail) {
			return false;
		}
		m_Head = TempHead;
		Data = m_Queue[TempHead];
		m_CurrentDataCount--;
		return true;
	}

	bool IsEmpty() {
		CThreadSync Sync(this);
		if (m_Head == m_Tail) {
			return true;
		}
		return false;
	}

public:
	inline size_t GetQueueSize() const { return m_CurrentDataCount; }

};