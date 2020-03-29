#include <iostream>
#include "array_doubly_linked_list.h"

int main() {
	CArrayDoublyLinkedList<int, 12> List;

	List.push_back(3, 8);
	List.push_back(4, 7);
	List.push_back(2, 5);
	List.push_back(1, 2);

	int data;
	List.pop_back(data);
	std::cout << data << std::endl;
	List.pop_back(data);
	std::cout << data << std::endl;
	List.pop_front(data);
	std::cout << data << std::endl;
	List.pop_front(data);
	std::cout << data << std::endl;

	return 0;
}