#include <iostream>
#include "LinkedList.h"
#include <list>

int main() {
	CLinkedList<int> LinkedList;

	LinkedList.push_back(3);
	LinkedList.push_back(4);
	LinkedList.push_back(5);
	LinkedList.push_front(2);
	LinkedList.push_front(1);

	std::cout << LinkedList.pop_front() << std::endl;
	std::cout << LinkedList.pop_front() << std::endl;
	std::cout << LinkedList.pop_front() << std::endl;
	std::cout << LinkedList.pop_front() << std::endl;
	std::cout << LinkedList.pop_front() << std::endl;

	return 0;
}