#include <iostream>
#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>

std::mutex g_StreamLock;
HANDLE g_hSemaphore = INVALID_HANDLE_VALUE;
int Count = 0;

void Thread(int ThreadNumber) {
	g_StreamLock.lock();
	std::cout << "Thread Num : " << ThreadNumber << std::endl;
	g_StreamLock.unlock();

	while (Count < 50) {
		WaitForSingleObject(g_hSemaphore, INFINITE);
		int Temp = Count;
		Sleep(1000);
		Count = Temp + 1;
		std::cout << "# (" << ThreadNumber << ") : Count++ " << Count << std::endl;
		ReleaseSemaphore(g_hSemaphore, 1, NULL);
	}
}

int main() {
	if ((g_hSemaphore = CreateSemaphore(NULL, 1, 1, NULL)) == NULL) {
		std::cout << "Failed To Create Semaphore! - " << GetLastError() << std::endl;
		return -1;
	}

	std::vector<std::thread> Threads;
	for (size_t i = 0; i < 2; i++) {
		Threads.push_back(std::thread(Thread, i));
	}

	for (auto& It : Threads) {
		if (It.joinable()) {
			It.join();
		}
	}
	CloseHandle(g_hSemaphore);
	return 0;
}