#pragma once

#include "input/Key.h"
#include <algorithm>

class KeyUtility
{
public:
	/*
	 * @brief �C���N�������g���[�v
	 */
	static int TrgIncrement(KeyCode::Code code, int value, int max)
	{
		if (!Key::GetInstance()->IsTrg(code)) return value;
		return ++value % max;
	}

	/*
	 * @brief �f�N�������g���[�v
	 */
	static int TrgDecrement(KeyCode::Code code, int value, int max)
	{
		if (!Key::GetInstance()->IsTrg(code)) return value;
		return (value + (max - 1)) % max;
	}

	/*
	 * @brief �^�U�̐؂�ւ�
	 */
	static bool TrgBoolean(KeyCode::Code code, bool value)
	{
		return Key::GetInstance()->IsTrg(code) ? !value : value;
	}

	/*
	 * @brief ���l���Z
	 */
	template <class T>
	static T TrgAddValue(KeyCode::Code code, T value, T add, T max)
	{
		if (!Key::GetInstance()->IsTrg(code)) return value;
		return std::min(value + add, max);
	}
	template <class T>
	static T PressAddValue(KeyCode::Code code, T value, T add, T max)
	{
		if (!Key::GetInstance()->IsPress(code)) return value;
		return std::min(value + add, max);
	}

	/*
	* @brief ���l���Z
	*/
	template <class T>
	static T TrgSubValue(KeyCode::Code code, T value, T sub, T min)
	{
		if (!Key::GetInstance()->IsTrg(code)) return value;
		return std::max(value - sub, min);
	}
	template <class T>
	static T PressSubValue(KeyCode::Code code, T value, T sub, T min)
	{
		if (!Key::GetInstance()->IsPress(code)) return value;
		return std::max(value - sub, min);
	}
};
