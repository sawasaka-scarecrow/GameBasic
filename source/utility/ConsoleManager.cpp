#include "ConsoleManager.h"
#include <io.h>
#include <Fcntl.h>
#include <stdio.h>

/*!
 * @brief �R���\�[���E�C���h�E���J��
 */
void ConsoleManager::Open()
{
	if (handle_ != 0) return ;

    AllocConsole();
    handle_ = GetStdHandle(STD_OUTPUT_HANDLE);
}

/*!
 * @brief �R���\�[���E�C���h�E�����
 */
void ConsoleManager::Close()
{
	if (handle_ != 0) {
		FreeConsole();
	}
	handle_ = 0;
}

/*!
 * @brief �R���\�[���E�C���h�E�ɕ������o��
 */
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
	WriteConsoleA(handle_, text, strlen(text), &dwWriteByte, nullptr);
}

/*!
 * @brief �R���\�[���E�C���h�E�ɕ������o��
 */
void ConsoleManager::PrintLine(const char* s, ...)
{
	if (handle_ == 0) return;

	// �e�L�X�g�t�H�[�}�b�g��ϊ�
	char text[1024];
	va_list arg;
	va_start(arg, s);
	vsprintf(text, s, arg);
	va_end(arg);
	strcat(text, "\n");

	DWORD dwWriteByte;
	WriteConsoleA(handle_, text, strlen(text), &dwWriteByte, nullptr);
}
