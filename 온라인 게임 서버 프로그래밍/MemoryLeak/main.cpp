#include "MemoryLeak.h"

int main() {
	char* pData = new char[1024];

//	delete[] pData;
	return 0;
}