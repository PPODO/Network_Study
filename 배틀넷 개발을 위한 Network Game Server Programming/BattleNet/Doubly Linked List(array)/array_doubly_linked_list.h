#pragma once
#include <array>

template<typename type>
struct NODE {
public:
	size_t m_nextIndex, m_prevIndex;
	type m_data;

public:
	NODE() : m_nextIndex(-1), m_prevIndex(-1), m_data() {};
	NODE(const type& data) : m_nextIndex(-1), m_prevIndex(-1), m_data(data) {};
	NODE(const type& data, size_t nextIndex, size_t prevIndex) : m_nextIndex(nextIndex), m_prevIndex(prevIndex), m_data(data) {};

};

template<typename type, size_t MAX_LENGTH = 24>
class CArrayDoublyLinkedList {
public:
	CArrayDoublyLinkedList() : m_headIndex(ULLONG_MAX), m_tailIndex(ULLONG_MAX) {};
	~CArrayDoublyLinkedList() {};

public:
	void push_back(const type& data, size_t Index);
	void push_front(const type& data, size_t Index);
	void pop_front(type& data);
	void pop_back(type& data);

private:
	std::array<NODE<type>, MAX_LENGTH> m_Array;
	size_t m_headIndex, m_tailIndex;

};

template<typename type, size_t MAX_LENGTH>
inline void CArrayDoublyLinkedList<type, MAX_LENGTH>::push_back(const type& data, size_t Index) {
	if (Index < m_Array.size()) {
		if (m_headIndex == ULLONG_MAX) {
			m_tailIndex = m_headIndex = Index;
			m_Array[Index] = NODE<type>(data);
		}
		else {
			m_Array[Index] = NODE<type>(data, 0, m_tailIndex);
			m_tailIndex = m_Array[m_tailIndex].m_nextIndex = Index;
		}
	}
}

template<typename type, size_t MAX_LENGTH>
inline void CArrayDoublyLinkedList<type, MAX_LENGTH>::push_front(const type& data, size_t Index) {
	if (Index < m_Array.size()) {
		if (m_headIndex == ULLONG_MAX) {
			m_headIndex = m_tailIndex = Index;
			m_Array[Index] = NODE<type>(data);
		}
		else {
			m_Array[Index] = NODE<type>(data, m_headIndex, 0);
			m_headIndex = m_Array[m_headIndex].m_prevIndex = Index;
		}
	}
}

template<typename type, size_t MAX_LENGTH>
inline void CArrayDoublyLinkedList<type, MAX_LENGTH>::pop_front(type& data) {
	if (m_headIndex == ULLONG_MAX) { return; }
	
	data = m_Array[m_headIndex].m_data;
	if (m_Array[m_headIndex].m_nextIndex == 0) {
		m_headIndex = m_tailIndex = ULLONG_MAX;
		return;
	}
	m_headIndex = m_Array[m_headIndex].m_nextIndex;
}

template<typename type, size_t MAX_LENGTH>
inline void CArrayDoublyLinkedList<type, MAX_LENGTH>::pop_back(type& data) {
	if (m_tailIndex == ULLONG_MAX) { return; }

	data = m_Array[m_tailIndex].m_data;
	if (m_Array[m_tailIndex].m_prevIndex == 0) {
		m_headIndex = m_tailIndex = ULLONG_MAX;
		return;
	}
	m_tailIndex = m_Array[m_tailIndex].m_prevIndex;
}