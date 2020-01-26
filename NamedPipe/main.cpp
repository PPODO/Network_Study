#include <iostream>
#include <Windows.h>

int main() {
	
	// 리눅스와 마찬가지로 파일로 존재하며, 원격 프로세스와도 통신할 수 있다는 특징을 가지고 있다.(WAN에서는 속도가 느리기 때문에 주로 LAN에서만 사용함)
	// 첫 번째 인수로 넘어가는 파이프 이름으로는 다음의 포맷을 따른다 -> \\pc name\pipe\pipe\name
	//CreateNamedPipe();

	// 파이프를 생성한 서버는 클라이언트의 연결을 기다린다.
	// WaitNamedPipe 함수와 ConnectNamedPipe 두 함수로 연결을 할 수 있는데,
	// WaitNamedPipe는 사용할 수 있는 파이프가 없더라도 nTimeOut 동안 클라이언트가 종료되길 기다리지만
	// ConnectNamedPipe함수는 곧바로 연결을 종료한다.
	//ConnectNamedPipe();
	//WaitNamedPipe();

	// 파이프의 종료는 CloseHandle함수를 사용한다.

	return 0;
}