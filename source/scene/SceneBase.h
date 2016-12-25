#pragma once

/*!
 * @brief �V�[���̏������p�����[�^�̊��N���X
 */
class SceneBaseParam
{
public :
	virtual ~SceneBaseParam(){}
};

/*!
 * @brief �V�[���̊��N���X
 */
class SceneBase
{
public :
	virtual ~SceneBase(){}

	/*!
	 * @brief ������
	 */
	virtual void Initialize(const SceneBaseParam* param){}

	/*!
	 * @brief �I����
	 */
	virtual void Finalize(){}

	/*!
	 * @brief �X�V
	 *
	 * @param df �O�̃t���[���Ƃ̍�������
	 */
	virtual void Update(float df){}

	/*!
	 * @brief �`��
	 */
	virtual void Render(){}
};
