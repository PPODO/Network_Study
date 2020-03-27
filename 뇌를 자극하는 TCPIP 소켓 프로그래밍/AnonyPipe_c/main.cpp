#include <iostream>
#include <Windows.h>

int main(int argc, char** argv) {
	HANDLE hReadPipe = reinterpret_cast<HANDLE>(atoi(argv[1]));

	while (true) {
		char MessageBuffer[1024] = { "\0" };
		DWORD RecvBytes;
		if (!ReadFile(hReadPipe, MessageBuffer, 1024, &RecvBytes, nullptr) || strncmp("quit", MessageBuffer, 4) == 0) {
			std::cout << "Error : " << GetLastError() << std::endl;
			break;
		}
		std::cout << "--> " << MessageBuffer << std::endl;
	}

	return 0;
}