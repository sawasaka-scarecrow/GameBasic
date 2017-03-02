#pragma once

#include "SceneBase.h"
#include <string>

/*!
 * @brief �V�[���̏������p�����[�^
 */
struct SceneGameParam : public SceneBaseParam
{
	std::string str;
};

/*!
 * @brief �������V�[��
 */
class SceneGame : public SceneBase
{
public :
	~SceneGame(){}

	/*!
	 * @brief ������
	 */
	void Initialize(const SceneBaseParam* param);

	/*!
	 * @brief �I����
	 */
	void Finalize();

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

private :
	SceneGameParam param_;
};
