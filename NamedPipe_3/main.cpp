#include <iostream>
#include <Windows.h>
#include <string>

const char* const PIPE_NAME = "\\\\.\\pipe\\echo";
const size_t BUFFER_SIZE = 1024;

int main() {
	HANDLE hNamedPipe = CreateFile(PIPE_NAME, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		std::cout << "Failed To Open Named Pipe File!\n";
		return -1;
	}

	while (true) {
		std::string Input;
		std::cout << "> ";
		std::getline(std::cin, Input);
		if (Input.find("quit") != std::string::npos) {
			break;
		}
		DWORD SendBytes;
		if (WriteFile(hNamedPipe, Input.c_str(), Input.length(), &SendBytes, nullptr)) {
			char MessageBuffer[BUFFER_SIZE] = { "\0" };
			DWORD RecvBytes;
			if (!ReadFile(hNamedPipe, MessageBuffer, BUFFER_SIZE, &RecvBytes, nullptr)) {
				std::cout << "Failed To Recv Message!\n";
				continue;
			}
			std::cout << "Server -> " << MessageBuffer << std::endl;
			continue;
		}
		std::cout << "Failed To Send Message!\n";
	}
	CloseHandle(hNamedPipe);
	return 0;
}