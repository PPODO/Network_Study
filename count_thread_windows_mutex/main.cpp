#include <iostream>
#include <Windows.h>

constexpr size_t MAX_THREAD = 4;
HANDLE g_hMutexObject;

DWORD WINAPI Thread_Function(void* Data) {
	int* Count = reinterpret_cast<int*>(Data);

	while (*Count < 20) {
		// 커널 모드 동기화는 문제점이 많은가 보다.
		// 제대로 작동하질 않음. 속도 때문인가?
		// 뭐든간에 유저 모드에선 잘되는거로 봐서 속도 문제이긴 한듯?.
		WaitForSingleObject(g_hMutexObject, INFINITE);
		std::cout << GetCurrentThreadId() << '\t' << *Count << std::endl;
		*Count += 1;
		ReleaseMutex(g_hMutexObject);
		Sleep(100);
	}

	return 0;
}

int main() {
	HANDLE Threads[MAX_THREAD];
	int Count = 0;

	if (!CreateMutex(NULL, false, NULL)) {
		std::cout << "Failed To Create Mutex!\n";
		return -1;
	}

	for (int i = 0; i < MAX_THREAD; i++) {
		Threads[i] = CreateThread(NULL, 0, Thread_Function, &Count, 0, NULL);
	}

	WaitForMultipleObjects(MAX_THREAD, Threads, true, INFINITE);

	for (int i = 0; i < MAX_THREAD; i++) {
		CloseHandle(Threads[i]);
	}
	CloseHandle(g_hMutexObject);

	return 0;
}