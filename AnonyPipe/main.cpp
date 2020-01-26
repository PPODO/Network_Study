#include <iostream>
#include <Windows.h>

int main() {
	
	// 파이프를 만들기 위해 사용하는 함수.
	// 첫 번째 인수 : 읽기 전용 파이프 핸들
	// 두 번째 인수 : 쓰기 전용 파이프 핸들
	// 세 번째 인수 : 보안 속성
	// 네 번째 인수 : 파이프 버퍼의 크기. 보내려는 메시지에 따라 사이즈를 따로 설정해 줄 수 있다. 기본은 0(기본 버퍼 크기)을 사용
	HANDLE hReadPipe, hWirtePipe;
	
	// 보안 속성을 설정하기 위한 구조체
	/*
	typedef struct _SECURITY_ATTRIBUTES {
		DWORD nLength;	// 구조체의 크기
		LPVOID lpSecurityDescriptor;	// 보안 등급 설정자
		BOOL bInheritHandle;	// 파이프를 자식프로세스에게 상속할 건지에 대한 값
	} SECURITY_ATTRIBUTES, * PSECURITY_ATTRIBUTES, * LPSECURITY_ATTRIBUTES;
	*/
	SECURITY_ATTRIBUTES Attribute;
	Attribute.nLength = sizeof(SECURITY_ATTRIBUTES);
	Attribute.lpSecurityDescriptor = nullptr;
	Attribute.bInheritHandle = true;

	CreatePipe(&hReadPipe, &hWirtePipe, &Attribute, 0);

	return 0;
}