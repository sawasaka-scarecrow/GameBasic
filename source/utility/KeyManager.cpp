#include "KeyManager.h"
#include <string.h>
#include <algorithm>
#include "master/MasterData.hpp"

KeyManager::KeyManager()
{
	Clear();
}

/*!
 * @brief �N���A
 */
void KeyManager::Clear()
{
	memset(infos_, false , sizeof(infos_));
}

/*!
 * @brief ��������
 */
void KeyManager::Down(unsigned char key)
{
	infos_[key].key_temp = true;
}

/*!
 * @brief ��������
 */
void KeyManager::Up(unsigned char key)
{
	infos_[key].key_temp = false;
}

/*!
 * @brief �L�[�̍X�V
 */
void KeyManager::Update()
{
	for (int i = 0; i < sizeof(infos_) / sizeof(*infos_); ++i) {
		auto& info = infos_[i];
		info.key_back = info.key;
		info.key = info.key_temp;
		info.key_count = info.key ? info.key_count + 1 : 0;
	}
}

/*!
 * @brief �L�[�̉�������ςȂ����擾
 */
bool KeyManager::IsPress(unsigned char key) const
{
	return infos_[key].key;
}

/*!
 * @brief �L�[�̉����������擾
 */
bool KeyManager::IsTrg(unsigned char key) const
{
	return infos_[key].key && !infos_[key].key_back;
}

/*!
 * @brief �L�[�̗����������擾
 */
bool KeyManager::IsRelease(unsigned char key) const
{
	return !infos_[key].key && infos_[key].key_back;
}

/*!
 * @brief �L�[���s�[�g
 */
bool KeyManager::IsRepeat(unsigned char key) const
{
	auto& info = infos_[key];
	if (info.key_count == 0) return false;
	if (info.key_count >= MasterData::KeyRepeatBase[MasterData::KeyRepeatBase.size() - 1].value) {
		return info.key_count % 2 == 0;
	}
	for (size_t i = 0; i < MasterData::KeyRepeatBase.size(); ++i) {
		if (info.key_count == MasterData::KeyRepeatBase[i].value) {
			return true;
		}
	}
	return false;
}
