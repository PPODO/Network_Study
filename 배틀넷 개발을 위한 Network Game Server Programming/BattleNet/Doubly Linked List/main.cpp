#include <iostream>
#include "doubly_linked_list.h"

int main() {
	CDoublyLinkedList<int> List;

	List.push_back(4);
	List.push_back(5);
	List.push_back(3);
	List.push_back(2);
	List.push_back(1);

	std::cout << List.pop_back() << std::endl;
	std::cout << List.pop_back() << std::endl;
	std::cout << List.pop_back() << std::endl;
	std::cout << List.pop_front() << std::endl;
	std::cout << List.pop_front() << std::endl;

	return 0;
}