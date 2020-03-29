#include <iostream>
#include "queue.h"
#include "../LinearQueue/queue.h"
#include <chrono>

int main() {
	{
		CLinearQueue<int, 8192> LinearQueue;
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

		for (size_t i = 0; i < 8192; i++) {
			LinearQueue.push_back(i + 1);
		}

		for (size_t i = 0; i < 8192; i++) {
			int data;
			LinearQueue.pop_front(data);
		}
		std::cout << "Elapsed Time(Linear Queue) : " << (std::chrono::system_clock::now() - start).count() << std::endl;
	}

	{
		CCircularQueue<int, 8192> CircularQueue;
		std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

		for (size_t i = 0; i < 8192; i++) {
			CircularQueue.push_back(i + 1);
		}

		for (size_t i = 0; i < 8192; i++) {
			int data;
			CircularQueue.pop_front(data);
		}
		std::cout << "Elapsed Time(Circular Queue) : " << (std::chrono::system_clock::now() - start).count() << std::endl;
	}

	return 0;
}