#include <iostream>
#include <Windows.h>
#include <process.h>

constexpr size_t ARRAY_SIZE = 100;
constexpr size_t MAX_THREAD = 4;

struct Data_Info {
	int* m_Address;
	int m_Index;
};

int g_DataArray[ARRAY_SIZE];
int g_SumArray[MAX_THREAD];
CRITICAL_SECTION g_CriticalSection;
CONDITION_VARIABLE g_ConditionalVariable;

void Thread_Function(void* Data) {
	Data_Info* Information = reinterpret_cast<Data_Info*>(Data);

	EnterCriticalSection(&g_CriticalSection);
	// 조건 변수가 설정된 후, 잠금이 해제되어 다른 스레드에서도 접근가능 한 상태가 됨.
	SleepConditionVariableCS(&g_ConditionalVariable, &g_CriticalSection, INFINITE);
	LeaveCriticalSection(&g_CriticalSection);

	int Sum = 0;
	const int LoopCount = ARRAY_SIZE / MAX_THREAD;
	for (int i = 0; i < LoopCount; i++) {
		Sum += Information->m_Address[(Information->m_Index * LoopCount) + i];
	}
	std::cout << Information->m_Index << '\t' << Sum << std::endl;
	g_SumArray[Information->m_Index] = Sum;
}

int main() {
	HANDLE Threads[MAX_THREAD];
	Data_Info Informations[MAX_THREAD];

	InitializeCriticalSection(&g_CriticalSection);
	InitializeConditionVariable(&g_ConditionalVariable);

	for (int i = 0; i < MAX_THREAD; i++) {
		Informations[i].m_Address = g_DataArray;
		Informations[i].m_Index = i;
		Threads[i] = reinterpret_cast<HANDLE>(_beginthread(Thread_Function, 0, &Informations[i]));
		Sleep(10);
	}

	for (int i = 0; i < ARRAY_SIZE; i++) {
		*(g_DataArray + i) = i;
	}

	WakeAllConditionVariable(&g_ConditionalVariable);

	WaitForMultipleObjects(MAX_THREAD, Threads, true, INFINITE);
	int Sum = 0;
	for (int i = 0; i < MAX_THREAD; i++) {
		CloseHandle(Threads[i]);
		Sum += g_SumArray[i];
	}

	std::cout << "Total Value(0 - 99) : " << Sum << std::endl;
	DeleteCriticalSection(&g_CriticalSection);
	return 0;
}