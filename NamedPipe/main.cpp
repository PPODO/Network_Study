#include <iostream>
#include <Windows.h>

int main() {
	
	// �������� ���������� ���Ϸ� �����ϸ�, ���� ���μ����͵� ����� �� �ִٴ� Ư¡�� ������ �ִ�.(WAN������ �ӵ��� ������ ������ �ַ� LAN������ �����)
	// ù ��° �μ��� �Ѿ�� ������ �̸����δ� ������ ������ ������ -> \\pc name\pipe\pipe\name
	//CreateNamedPipe();

	// �������� ������ ������ Ŭ���̾�Ʈ�� ������ ��ٸ���.
	// WaitNamedPipe �Լ��� ConnectNamedPipe �� �Լ��� ������ �� �� �ִµ�,
	// WaitNamedPipe�� ����� �� �ִ� �������� ������ nTimeOut ���� Ŭ���̾�Ʈ�� ����Ǳ� ��ٸ�����
	// ConnectNamedPipe�Լ��� ��ٷ� ������ �����Ѵ�.
	//ConnectNamedPipe();
	//WaitNamedPipe();

	// �������� ����� CloseHandle�Լ��� ����Ѵ�.

	return 0;
}