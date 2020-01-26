#include <iostream>
#include <string>
#include <Windows.h>

int main(int argc, char** argv) {
	HANDLE hReadPipe, hWritePipe;
	SECURITY_ATTRIBUTES Attribute;
	Attribute.nLength = sizeof(Attribute);
	Attribute.lpSecurityDescriptor = nullptr;
	Attribute.bInheritHandle = true;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &Attribute, 0)) {
		std::cout << "Failed To Create AnonyPipe!\n";
		return -1;
	}

	TCHAR Cmd[128] = { L"\0" };
	_snwprintf_s(Cmd, 128, _TRUNCATE, L"C:\\Users\\sherl\\source\\repos\\NetworkStudy_Window\\x64\\Release\\AnonyPipe_c.exe %d", hReadPipe);

	STARTUPINFO StartInfo{ 0, };
	PROCESS_INFORMATION ProcessInfo{ 0, };
	StartInfo.cb = sizeof(StartInfo);

	if (!CreateProcess(NULL, Cmd, NULL, NULL, true, CREATE_NEW_CONSOLE, NULL, NULL, &StartInfo, &ProcessInfo)) {
		std::cout << "Failed To Create Process! : " << GetLastError() << std::endl;
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		return -1;
	}

	while (true) {
		std::string Input;
		DWORD WriteBytes;
		std::cout << "> ";
		std::getline(std::cin, Input);
		if (!WriteFile(hWritePipe, Input.c_str(), Input.length(), &WriteBytes, nullptr) || Input.find("quit") != std::string::npos) {
			break;
		}
	}

	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	return 0;
}