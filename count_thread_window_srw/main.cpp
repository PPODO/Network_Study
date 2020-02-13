#include <iostream>
#include <Windows.h>

// ���� ���� ������ ��
// Critical Section���� �������� �б⸸ �ϴ� ������� ���⸸ �ϴ� �����带 �����ؼ� �����Ѵٴ� ��.
// �����ڿ��� �����͸� �о ���� '����'���� �ʱ� ������ ���ÿ� �����ص� ����������, �����͸� ���� ��쿡�� ��Ⱑ �޶�����.

constexpr size_t MAX_THREAD = 4;
SRWLOCK g_LockObject;

DWORD WINAPI Thread_Function(void* Data) {
	int* Count = reinterpret_cast<int*>(Data);

	while (*Count < 20) {
		// �����ڿ��� �б� ���� Lock
		AcquireSRWLockShared(&g_LockObject);
		int Temp = *Count;
		ReleaseSRWLockShared(&g_LockObject);
		Temp += 1;
		// �����ڿ��� ���� ���� Lock
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