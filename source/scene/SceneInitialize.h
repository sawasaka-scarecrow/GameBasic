#pragma once

#include "SceneBase.h"

/*!
 * @brief �������V�[��
 */
class SceneInitialize : public SceneBase
{
public :
	~SceneInitialize(){}

	/*!
	 * @brief �X�V
	 *
	 * @param df �O�̃t���[���Ƃ̍�������
	 */
	void Update(float df);

	/*!
	 * @brief �`��
	 */
	void Render2D();
};
