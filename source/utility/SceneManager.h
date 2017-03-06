/*!
 * @file
 *
 * @brief �V�[���Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include "StateManager.h"
#include "scene/SceneBase.h"
#include "scene/SceneList.h"
#include <memory>
#include <stack>

/*!
 * @brief �V�[���Ǘ��N���X
 */
class SceneManager
{
public :
	/*!
	 * @brief �I��
	 */
	void Finalize();

	/*!
	 * @brief �V�[����؂�ւ���
	 *
	 * @param scene �؂�ւ���V�[��
	 * @param param �n���p�����[�^
	 */
	void Change(SceneList::List scene, std::shared_ptr<SceneBaseParam> param);

	/*!
	 * @brief �O�̃V�[���ɖ߂�
	 *
	 * @return �O�̃V�[�����Ȃ��ꍇ�� false ��Ԃ��܂��B
	 */
	bool Back();

	/*!
	 * @brief �V�[���̃X�^�b�N���N���A����
	 */
	void Clear();

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
	 * @brief 3D�`��
	 */
	void Render3D();

	/*!
	 * @brief 2D�`��
	 */
	void Render2D();

	static SceneManager* GetInstance() {
		static SceneManager v;
		return &v;
	}
private :
	SceneManager() : last_scene_(SceneList::None), is_restart_(false) {}

	struct StackInfo {
		SceneList::List last_scene_;
		std::shared_ptr<SceneBaseParam> last_param_;
	};

	StateManager<SceneList::List> scene_state_;
	std::shared_ptr<SceneBase> scene_;
	SceneList::List last_scene_;
	std::shared_ptr<SceneBaseParam> last_param_;
	std::stack<StackInfo> stack_;

	bool is_restart_;
};
