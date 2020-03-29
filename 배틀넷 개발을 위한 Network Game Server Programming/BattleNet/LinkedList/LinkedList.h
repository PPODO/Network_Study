#pragma once
#include <memory>

template<typename type>
struct NODE {
public:
	NODE* m_next;
	type m_data;

public:
	NODE() : m_next(), m_data() {};
	NODE(type DATA) : m_data(DATA), m_next() {};
	NODE(type DATA, NODE* NEXT) : m_data(DATA), m_next(NEXT) {};
};

template<typename type>
class CLinkedList {
public:
	CLinkedList() : m_head(), m_tail() {}
	~CLinkedList();

public:
	NODE<type>* push_front(const type& data);
	NODE<type>* push_back(const type& data);
	type pop_front();

private:
	NODE<type>* m_head, *m_tail;

};

template<typename type>
inline CLinkedList<type>::~CLinkedList() {
	while (m_head != nullptr) {
		auto deleteptr = m_head;
		m_head = m_head->m_next;
		delete deleteptr;
	}
}

template<typename type>
inline NODE<type>* CLinkedList<type>::push_front(const type& data) {
	auto NewNode = new NODE<type>(data, m_head);
	return (m_head = NewNode);
}

template<typename type>
inline NODE<type>* CLinkedList<type>::push_back(const type& data) {
	if (m_head) {
		m_tail->m_next = new NODE<type>(data);
		m_tail = m_tail->m_next;
	}
	else {
		m_head = new NODE<type>(data);
		m_tail = m_head;
	}
	return m_tail;
}

template<typename type>
inline type CLinkedList<type>::pop_front() {
	if (!m_head) { return type(); }

	auto deletePtr = m_head;
	auto data = m_head->m_data;
	m_head = m_head->m_next;
	delete deletePtr;

	return data;
}
