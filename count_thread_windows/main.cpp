#include <iostream>
#include <Windows.h>
#include <process.h>

constexpr size_t MAX_THREAD = 4;

/*void Thread_Function1(void* Data) {
	int* Count = reinterpret_cast<int*>(Data);

	while (*Count < 20) {
		std::cout << GetCurrentThreadId() << '\t' << *Count << std::endl;
		*Count += 1;
		Sleep(1000);
	}
}*/

DWORD WINAPI Thread_Function(void* Data) {
	int* Count = reinterpret_cast<int*>(Data);

	while (*Count < 20) {
		std::cout << GetCurrentThreadId() << '\t' << *Count << std::endl;
		*Count += 1;
		Sleep(1000);
	}

	return 0;
}

int main() {

	HANDLE Threads[MAX_THREAD];
	int Count = 0;

	for (int i = 0; i < MAX_THREAD; i++) {
		Threads[i] = CreateThread(NULL, 0, Thread_Function, &Count, 0, NULL);
		//Threads[i] = reinterpret_cast<HANDLE>(_beginthread(Thread_Function1, 0, &Count));
		//Threads[i] = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, Thread_Function1, &Count, 0, nullptr));
	}

	WaitForMultipleObjects(MAX_THREAD, Threads, true, INFINITE);

	for (int i = 0; i < MAX_THREAD; i++) {
		CloseHandle(Threads[i]);
	}
	return 0;
}