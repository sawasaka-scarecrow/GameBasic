/*!
 * @file
 *
 * @brief �R���\�[���Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include <Windows.h>

/*!
 * @brief �R���\�[���Ǘ��N���X
 */
class ConsoleManager
{
public :
	/*!
	 * @brief �R���\�[���E�C���h�E���J��
	 */
	void Open();

	/*!
	 * @brief �R���\�[���E�C���h�E�����
	 */
	void Close();

	/*!
	 * @brief �R���\�[���E�C���h�E�ɕ������o��
	 */
	void Print(const char* s, ...);

	static ConsoleManager* GetInstance() {
		static ConsoleManager v;
		return &v;
	}

private :
	ConsoleManager() : handle_(0){}
	HANDLE handle_;
};
