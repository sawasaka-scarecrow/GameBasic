#pragma once

#include "StateManager.h"
#include "scene/SceneBase.h"
#include "scene/SceneList.h"
#include <memory>

class SceneManager
{
public :
	/*!
	 * @brief �V�[����؂�ւ���
	 *
	 * @param scene �؂�ւ���V�[��
	 * @param param �n���p�����[�^
	 */
	void Change(SceneList::List scene, std::shared_ptr<SceneBaseParam> param);

	/*!
	 * @brief �V�[�����ŏ����珈������
	 */
	void Restart();

	/*!
	 * @brief �X�V
	 *
	 * @param df �O�̃t���[���Ƃ̍�������
	 */
	void Update(float df);

	/*!
	* @brief �`��
	*/
	void Render();

	static SceneManager* GetInstance()
	{
		static SceneManager v;
		return &v;
	}
private :
	SceneManager() : is_restart_(false) {}

	StateManager<SceneList::List> scene_state_;
	std::shared_ptr<SceneBase> scene_;
	std::shared_ptr<SceneBaseParam> param_;
	bool is_restart_;
};
