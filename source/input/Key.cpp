#include "Key.h"
#include <string.h>
#include <algorithm>
#include "master/MasterData.hpp"

Key::Key()
{
	Clear();
}

/*!
 * @brief �N���A
 */
void Key::Clear()
{
	memset(infos_, false , sizeof(infos_));
}

/*!
 * @brief ��������
 */
void Key::Down(unsigned char key)
{
	infos_[key].key_temp = true;
}

/*!
 * @brief ��������
 */
void Key::Up(unsigned char key)
{
	infos_[key].key_temp = false;
}

/*!
 * @brief �L�[�̍X�V
 */
void Key::Update()
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
bool Key::IsPress(unsigned char key) const
{
	return infos_[key].key;
}

/*!
 * @brief �L�[�̉����������擾
 */
bool Key::IsTrg(unsigned char key) const
{
	return infos_[key].key && !infos_[key].key_back;
}

/*!
 * @brief �L�[�̗����������擾
 */
bool Key::IsRelease(unsigned char key) const
{
	return !infos_[key].key && infos_[key].key_back;
}

/*!
 * @brief �L�[���s�[�g
 */
bool Key::IsRepeat(unsigned char key) const
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
