#include <iostream>
#include <Windows.h>

int main() {
	
	// �������� ����� ���� ����ϴ� �Լ�.
	// ù ��° �μ� : �б� ���� ������ �ڵ�
	// �� ��° �μ� : ���� ���� ������ �ڵ�
	// �� ��° �μ� : ���� �Ӽ�
	// �� ��° �μ� : ������ ������ ũ��. �������� �޽����� ���� ����� ���� ������ �� �� �ִ�. �⺻�� 0(�⺻ ���� ũ��)�� ���
	HANDLE hReadPipe, hWirtePipe;
	
	// ���� �Ӽ��� �����ϱ� ���� ����ü
	/*
	typedef struct _SECURITY_ATTRIBUTES {
		DWORD nLength;	// ����ü�� ũ��
		LPVOID lpSecurityDescriptor;	// ���� ��� ������
		BOOL bInheritHandle;	// �������� �ڽ����μ������� ����� ������ ���� ��
	} SECURITY_ATTRIBUTES, * PSECURITY_ATTRIBUTES, * LPSECURITY_ATTRIBUTES;
	*/
	SECURITY_ATTRIBUTES Attribute;
	Attribute.nLength = sizeof(SECURITY_ATTRIBUTES);
	Attribute.lpSecurityDescriptor = nullptr;
	Attribute.bInheritHandle = true;

	CreatePipe(&hReadPipe, &hWirtePipe, &Attribute, 0);

	return 0;
}