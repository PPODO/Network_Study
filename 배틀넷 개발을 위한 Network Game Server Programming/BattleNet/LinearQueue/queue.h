#pragma once
#include <iostream>
#include <array>

template<typename T, size_t QUEUE_LENGTH = 24>
class CLinearQueue {
public:
	CLinearQueue();
	~CLinearQueue();

public:
	void push_back(const T& Data);
	void pop_front(T& Data);

private:
	std::array<T, QUEUE_LENGTH> m_Queue;
	size_t m_Tail;

};

template<typename T, size_t QUEUE_LENGTH>
inline CLinearQueue<T, QUEUE_LENGTH>::CLinearQueue() : m_Tail() {}

template<typename T, size_t QUEUE_LENGTH>
inline CLinearQueue<T, QUEUE_LENGTH>::~CLinearQueue() {}

template<typename T, size_t QUEUE_LENGTH>
inline void CLinearQueue<T, QUEUE_LENGTH>::push_back(const T& Data) {
	if (m_Tail >= m_Queue.size()) { std::cout << "queue is full!"; return; }
	m_Queue[m_Tail++] = Data;
}

template<typename T, size_t QUEUE_LENGTH>
inline void CLinearQueue<T, QUEUE_LENGTH>::pop_front(T& Data) {
	if (m_Queue.size() == 0) { std::cout << "queue is empty!"; return; }

	--m_Tail;
	Data = m_Queue.front();
	for (size_t i = 0; i < m_Queue.size() - 1; i++) {
		m_Queue[i] = m_Queue[i + 1];
	}
}

