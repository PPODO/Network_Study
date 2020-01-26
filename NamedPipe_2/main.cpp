#include <iostream>
#include <Windows.h>

const char* const PIPE_NAME = "\\\\.\\pipe\\echo";
const size_t BUFFER_SIZE = 1024;

int main() {
	HANDLE hNamedPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, BUFFER_SIZE, BUFFER_SIZE, NMPWAIT_USE_DEFAULT_WAIT, nullptr);
	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		std::cout << "Failed To Create Named Pipe!\n";
		return -1;
	}
	
	while (true) {
		if (ConnectNamedPipe(hNamedPipe, nullptr)) {
			char MessageBuffer[BUFFER_SIZE] = { "\0" };
			DWORD RecvBytes;
			if (ReadFile(hNamedPipe, MessageBuffer, BUFFER_SIZE, &RecvBytes, nullptr)) {
				std::cout << "Recv Client -> " << MessageBuffer << std::endl;
				DWORD SendBytes;
				if (!WriteFile(hNamedPipe, MessageBuffer, RecvBytes, &SendBytes, nullptr)) {
					std::cout << "Send Failure!\n";
				}
				continue;
			}
			std::cout << "Recv Failure!\n";
		}
	}
	CloseHandle(hNamedPipe);
	return 0;
}