#pragma once

template<typename type>
struct NODE {
public:
	NODE* m_next, *m_prev;
	type m_data;

public:
	NODE() : m_next(), m_prev(), m_data() {};
	NODE(const type& data) : m_next(), m_prev(), m_data(data) {};
	NODE(const type& data, NODE* next, NODE* prev) : m_next(next), m_prev(prev), m_data(data) {};

};

template<typename type>
class CDoublyLinkedList {
public:
	CDoublyLinkedList() : m_head(), m_tail() {};
	~CDoublyLinkedList();

public:
	NODE<type>* push_front(const type& data);
	NODE<type>* push_back(const type& data);
	type pop_front();
	type pop_back();

private:
	NODE<type>* m_head, *m_tail;

};

template<typename type>
inline CDoublyLinkedList<type>::~CDoublyLinkedList() {
	while (m_head) {
		auto deletePtr = m_head;
		m_head = m_head->m_next;
		delete deletePtr;
	}
	m_head = m_tail = nullptr;
}

template<typename type>
inline NODE<type>* CDoublyLinkedList<type>::push_front(const type& data) {
	auto NewNode = new NODE<type>(data, m_head, nullptr);
	m_head = NewNode;
	return m_head;
}

template<typename type>
inline NODE<type>* CDoublyLinkedList<type>::push_back(const type& data) {
	if (m_head) {
		m_tail->m_next = new NODE<type>(data, nullptr, m_tail);
		m_tail = m_tail->m_next;
	}
	else {
		m_head = new NODE<type>(data, nullptr, nullptr);
		m_tail = m_head;
	}
	return m_tail;
}

template<typename type>
inline type CDoublyLinkedList<type>::pop_front() {
	auto data = m_head->m_data;
	auto deletePtr = m_head;

	m_head = m_head->m_next;
	if(m_head)
		m_head->m_prev = nullptr;

	delete deletePtr;

	return data;
}

template<typename type>
inline type CDoublyLinkedList<type>::pop_back() {
	auto data = m_tail->m_data;
	auto deletePtr = m_tail;

	m_tail = m_tail->m_prev;
	if(m_tail)
		m_tail->m_next = nullptr;

	delete deletePtr;

	return data;
}