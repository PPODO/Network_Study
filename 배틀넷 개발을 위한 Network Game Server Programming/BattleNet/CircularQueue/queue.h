#pragma once
#include <vector>
#include <array>

template<typename T, size_t QUEUE_LENGTH = 24>
class CCircularQueue {
public:
	CCircularQueue();
	~CCircularQueue();

public:
	void push_back(const T& data);
	void pop_front(T& data);

private:
	std::array<T, QUEUE_LENGTH> m_Queue;
	size_t m_Head, m_Tail;

};

template<typename T, size_t QUEUE_LENGTH>
inline CCircularQueue<T, QUEUE_LENGTH>::CCircularQueue() : m_Head(), m_Tail() {}

template<typename T, size_t QUEUE_LENGTH>
inline CCircularQueue<T, QUEUE_LENGTH>::~CCircularQueue() {}

template<typename T, size_t QUEUE_LENGTH>
inline void CCircularQueue<T, QUEUE_LENGTH>::push_back(const T& data) {
	m_Queue[m_Tail] = data;
	(m_Tail = (m_Tail + 1) % QUEUE_LENGTH);
}

template<typename T, size_t QUEUE_LENGTH>
inline void CCircularQueue<T, QUEUE_LENGTH>::pop_front(T& data) {
	data = m_Queue[m_Head];
	m_Head = (m_Head + 1) % QUEUE_LENGTH;
}