#include "ConsoleManager.h"
#include <io.h>
#include <Fcntl.h>
#include <stdio.h>

void ConsoleManager::Open()
{
    AllocConsole();
    handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
}

void ConsoleManager::Close()
{
	if (handle_ != 0) {
		FreeConsole();
	}
	handle_ = 0;
}

void ConsoleManager::Print(const char* s, ...)
{
	if (handle_ == 0) return;

	// �e�L�X�g�t�H�[�}�b�g��ϊ�
	char text[1024];
	va_list arg;
	va_start(arg, s);
	vsprintf(text, s, arg);
	va_end(arg);

	DWORD dwWriteByte;
	WriteConsoleA(handle_, text, strlen(text), &dwWriteByte, NULL);
}
