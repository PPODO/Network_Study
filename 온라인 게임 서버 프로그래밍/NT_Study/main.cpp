#include <Windows.h>
#include <iostream>

VOID* Fiber[4];

VOID Fiber0() {
	while (true) {
		for (INT i = 0; i < 4; i++) {
			SwitchToFiber(Fiber[i]);
		}
	}
}

VOID CALLBACK Fiber1(VOID*) {
	while (true) {
		std::cout << "Fiber 1\n";
		SwitchToFiber(Fiber[0]);
	}
}

VOID CALLBACK Fiber2(VOID*) {
	while (true) {
		std::cout << "Fiber 2\n";
		SwitchToFiber(Fiber[0]);
	}
}

VOID CALLBACK Fiber3(VOID*) {
	while (true) {
		std::cout << "Fiber 3\n";
		SwitchToFiber(Fiber[0]);
	}
}

int main() {
	Fiber[0] = ConvertThreadToFiber(NULL);
	Fiber[1] = CreateFiber(0, Fiber1, NULL);
	Fiber[2] = CreateFiber(0, Fiber2, NULL);
	Fiber[3] = CreateFiber(0, Fiber3, NULL);

	Fiber0();
	return 0;
}