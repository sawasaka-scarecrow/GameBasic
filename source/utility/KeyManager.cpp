#include "KeyManager.h"
#include <string.h>

KeyManager::KeyManager()
{
	Clear();
}

/*!
 * @brief ��������
 */
void KeyManager::Clear()
{
	memset(keys_, 0 , sizeof(keys_));
	memset(keys_back_, 0 , sizeof(keys_back_));
	memset(keys_temp_, 0 , sizeof(keys_temp_));
}

/*!
 * @brief ��������
 */
void KeyManager::Down(unsigned char key)
{
	keys_temp_[key] = true;
}

/*!
 * @brief ��������
 */
void KeyManager::Up(unsigned char key)
{
	keys_temp_[key] = false;
}

/*!
 * @brief �L�[�̍X�V
 */
void KeyManager::Update()
{
	memcpy(keys_back_, keys_, sizeof(keys_));
	memcpy(keys_, keys_temp_, sizeof(keys_));
}

/*!
 * @brief �L�[�̉�������ςȂ����擾
 */
bool KeyManager::IsPress(unsigned char key) const
{
	return keys_[key];
}

/*!
 * @brief �L�[�̉����������擾
 */
bool KeyManager::IsTrg(unsigned char key) const
{
	return keys_[key] && !keys_back_[key];
}

/*!
 * @brief �L�[�̗����������擾
 */
bool KeyManager::IsRelease(unsigned char key) const
{
	return !keys_[key] && keys_back_[key];
}
