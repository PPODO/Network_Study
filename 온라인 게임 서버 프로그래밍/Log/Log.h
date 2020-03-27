#pragma once
#include <cstdio>
#include <fstream>
#include <Windows.h>

#define MAX_BUFFER_LENGTH 256
#define MAX_DATETIME_LENGTH 32

class CLog {
public:
	static bool WriteLog(LPCTSTR Data, ...) {
		SYSTEMTIME SystemTime;
		TCHAR CurrentDate[MAX_DATETIME_LENGTH] = { L"\0" };
		TCHAR CurrentFileName[MAX_PATH] = { L"\0" };
		TCHAR DebugLog[MAX_BUFFER_LENGTH] = { L"\0" };
		std::wfstream File;
		va_list ap;
		TCHAR Log[MAX_BUFFER_LENGTH] = { L"\0" };

		va_start(ap, Data);
		vswprintf_s(Log, Data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);

		swprintf_s(CurrentDate, MAX_DATETIME_LENGTH, L"%d-%d-%d %d:%d:%d", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

		swprintf_s(CurrentFileName, MAX_PATH, L"LOG_%d-%d-%d %d.log", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour);

		File.open(CurrentFileName, std::ios::app);
		if (!File.is_open()) {
			return false;
		}

		File << L'[' << CurrentDate << L"] " << reinterpret_cast<const char*>(Log) << std::endl;
		swprintf_s(DebugLog, MAX_BUFFER_LENGTH, L"[%s] %S\n", CurrentDate, reinterpret_cast<const char*>(Log));

		File.close();

		OutputDebugString(DebugLog);
		wprintf(L"%s", DebugLog);
		return true;
	}

};