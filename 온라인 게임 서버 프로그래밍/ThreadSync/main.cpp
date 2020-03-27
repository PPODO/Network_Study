#include "CircularQueue.h"
#include "MemoryPool.h"
#include "MemoryLeak.h"
#include <thread>
#include <iostream>

CCircularQueue<int> Queue;

class Object : public CMemoryPool<Object> {
public:
	int A;
	float B;
};

int main() {

	std::thread T1 = std::thread([&]() {
		for (int i = 0; i < 50; i++) {
			Queue.Push(i + 100);
		}
	});

	for (int i = 0; i < 50; i++) {
		Queue.Push(i);
	}

	T1.join();

//	Object* AO = new Object;

//	delete AO;
	return 0;
}