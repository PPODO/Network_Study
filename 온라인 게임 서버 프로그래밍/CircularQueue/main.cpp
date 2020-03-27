#include <iostream>
#include <Windows.h>

const size_t MAX_QUEUE_LENGTH = 10;

template<typename T>
class CCircularQueue {
private:
	T m_Queue[MAX_QUEUE_LENGTH];
	size_t m_Head, m_Tail;


public:
	CCircularQueue() : m_Head(0), m_Tail(0) {
		ZeroMemory(m_Queue, sizeof(m_Queue));
	}

	~CCircularQueue() {};

public:
	bool Begin() {
		ZeroMemory(m_Queue, sizeof(m_Queue));
		m_Head = m_Tail = 0;
		
		return true;
	}

	bool End() { return true; }

	bool Push(const T& Data) {
		size_t TempTail = (m_Tail + 1) % MAX_QUEUE_LENGTH;

		if (TempTail == m_Head) {
			return false;
		}
		m_Queue[TempTail] = Data;
		m_Tail = TempTail;
		return true;
	}

	bool Pop(T& Data) {
		size_t TempHead = (m_Head + 1) % MAX_QUEUE_LENGTH;

		if (TempHead == m_Tail) {
			return false;
		}
		Data = m_Queue[TempHead];
		m_Head = TempHead;
		return true;
	}

	bool IsEmpty() {
		if (m_Head == m_Tail) {
			return true;
		}
		return false;
	}

};