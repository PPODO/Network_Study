#include <cstdio>
#include <Windows.h>
#include <DbgHelp.h>

LPTOP_LEVEL_EXCEPTION_FILTER PrevExceptionFilter;

typedef bool (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType, const PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, const PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, const PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LONG WINAPI UnHandledExceptionFilter(_EXCEPTION_POINTERS* ExceptionInfo) {
	HMODULE DllHandle = nullptr;

	DllHandle = LoadLibrary(L"DBGHELP.DLL");

	if (DllHandle) {
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump) {
			TCHAR DumpPath[MAX_PATH] = { L"\0" };
			SYSTEMTIME SystemTime;

			GetLocalTime(&SystemTime);

			swprintf(DumpPath, MAX_PATH, L"%d-%d-%d %d_%d_%d.dmp", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

			HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
			
			if (FileHandle != INVALID_HANDLE_VALUE) {
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = ExceptionInfo;
				MiniDumpExceptionInfo.ClientPointers = NULL;

				bool Success = Dump(GetCurrentProcess(), GetCurrentProcessId(), FileHandle, MiniDumpNormal, &MiniDumpExceptionInfo, nullptr, nullptr);

				if (Success) {
					CloseHandle(FileHandle);
					return EXCEPTION_EXECUTE_HANDLER;
				}
			}
			CloseHandle(FileHandle);
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

bool Begin() {
	SetErrorMode(SEM_FAILCRITICALERRORS);

	PrevExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return true;
}

bool End() {
	SetUnhandledExceptionFilter(PrevExceptionFilter);
	return true;
}

void UnHandledExceptionFunction() {
	int* a = new int;

	a = nullptr;
	*a = 10;
}

int main() {
	Begin();
	UnHandledExceptionFunction();
	End();
	return 0;
}