#include <iostream>
#include <Windows.h>

// 슬림 리더 라이터 락
// Critical Section과의 차이점은 읽기만 하는 스레드와 쓰기만 하는 스레드를 구분해서 관리한다는 점.
// 공유자원을 데이터를 읽어도 값을 '수정'하지 않기 때문에 동시에 접근해도 무방하지만, 데이터를 쓰는 경우에는 얘기가 달라진다.

constexpr size_t MAX_THREAD = 4;
SRWLOCK g_LockObject;

DWORD WINAPI Thread_Function(void* Data) {
	int* Count = reinterpret_cast<int*>(Data);

	while (*Count < 20) {
		// 공유자원을 읽기 위한 Lock
		AcquireSRWLockShared(&g_LockObject);
		int Temp = *Count;
		ReleaseSRWLockShared(&g_LockObject);
		Temp += 1;
		// 공유자원을 쓰기 위한 Lock
		AcquireSRWLockExclusive(&g_LockObject);
		std::cout << GetCurrentThreadId() << '\t' << *Count << std::endl;
		*Count = Temp;
		ReleaseSRWLockExclusive(&g_LockObject);
		Sleep(100);
	}

	return 0;
}

int main() {
	HANDLE Threads[MAX_THREAD];
	int Count = 0;

	InitializeSRWLock(&g_LockObject);

	for (int i = 0; i < MAX_THREAD; i++) {
		Threads[i] = CreateThread(NULL, 0, Thread_Function, &Count, 0, NULL);
	}

	WaitForMultipleObjects(MAX_THREAD, Threads, true, INFINITE);

	for (int i = 0; i < MAX_THREAD; i++) {
		CloseHandle(Threads[i]);
	}

	return 0;
}