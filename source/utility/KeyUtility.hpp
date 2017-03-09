#pragma once

#include "input/Key.h"
#include <algorithm>

class KeyUtility
{
public:
	/*
	 * @brief �C���N�������g���[�v
	 */
	static int TrgIncrement(char key, int value, int max)
	{
		if (!Key::GetInstance()->IsTrg(key)) return value;
		return ++value % max;
	}

	/*
	 * @brief �f�N�������g���[�v
	 */
	static int TrgDecrement(char key, int value, int max)
	{
		if (!Key::GetInstance()->IsTrg(key)) return value;
		return (value + (max - 1)) % max;
	}

	/*
	 * @brief �^�U�̐؂�ւ�
	 */
	static bool TrgBoolean(char key, bool value)
	{
		return Key::GetInstance()->IsTrg(key) ? !value : value;
	}

	/*
	 * @brief ���l���Z
	 */
	template <class T>
	static T TrgAddValue(char key, T value, T add, T max)
	{
		if (!Key::GetInstance()->IsTrg(key)) return value;
		return std::min(value + add, max);
	}
	template <class T>
	static T PressAddValue(char key, T value, T add, T max)
	{
		if (!Key::GetInstance()->IsPress(key)) return value;
		return std::min(value + add, max);
	}

	/*
	* @brief ���l���Z
	*/
	template <class T>
	static T TrgSubValue(char key, T value, T sub, T min)
	{
		if (!Key::GetInstance()->IsTrg(key)) return value;
		return std::max(value - sub, min);
	}
	template <class T>
	static T PressSubValue(char key, T value, T sub, T min)
	{
		if (!Key::GetInstance()->IsPress(key)) return value;
		return std::max(value - sub, min);
	}
};
