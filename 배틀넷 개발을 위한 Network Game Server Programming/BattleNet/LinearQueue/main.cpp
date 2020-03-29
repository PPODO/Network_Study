#include <iostream>
#include "queue.h"

int main() {
	CLinearQueue<int> queue;

	queue.push_back(1);
	queue.push_back(2);
	queue.push_back(3);
	queue.push_back(4);

	int data;
	queue.pop_front(data);
	std::cout << data << std::endl;
	queue.pop_front(data);
	std::cout << data << std::endl;
	queue.pop_front(data);
	std::cout << data << std::endl;
	queue.pop_front(data);
	std::cout << data << std::endl;

	return 0;
}