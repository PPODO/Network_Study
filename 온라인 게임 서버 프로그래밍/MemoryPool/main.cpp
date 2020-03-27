#include "MemoryPool.h"
#include <vector>

class A : public CMemoryPool<A> {
public:
	std::vector<int> a;

public:
	A() {};

};

int main() {
	

	return 0;
}