#include "SceneManager.h"
#include "scene/SceneBase.h"
#include "scene/SceneInitialize.h"
#include "scene/SceneTitle.h"
#include "scene/SceneOption.h"
#include "scene/SceneGame.h"

/*!
 * @brief �V�[���؂�ւ�
 */
void SceneManager::Change(SceneList::List scene, std::shared_ptr<SceneBaseParam> param)
{
	if (last_scene_ != SceneList::None) {
		StackInfo info = {
			last_scene_,
			last_param_,
		};
		stack_.push(info);
	}

	last_scene_ = scene;
	last_param_ = param;
	scene_state_.Change(last_scene_);
}

/*!
 * @brief �V�[���؂�ւ�
 */
bool SceneManager::Back()
{
	if (stack_.empty()) return false;

	last_scene_ = stack_.top().last_scene_;
	last_param_ = stack_.top().last_param_;
	stack_.pop();
	scene_state_.Change(last_scene_);

	return true;
}

/*!
 * @brief �V�[���̃X�^�b�N���N���A����
 */
void SceneManager::Clear()
{
	std::stack<StackInfo>().swap(stack_);
}


/*!
 * @brief �V�[�����ŏ����珈������
 */
void SceneManager::Restart()
{
	is_restart_ = true;
}

/*!
 * @brief �X�V
 */
void SceneManager::Update(float df)
{
	if (scene_state_.Update(df) || is_restart_) {
		is_restart_ = false;
		if (scene_) {
			scene_->Finalize();
		}
		switch (scene_state_) {
		case SceneList::Initialize :	scene_.reset(new SceneInitialize());	break;
		case SceneList::Title :			scene_.reset(new SceneTitle());			break;
		case SceneList::Option :		scene_.reset(new SceneOption());		break;
		case SceneList::Game :			scene_.reset(new SceneGame());			break;
		default : break;
		}

		if (scene_) {
			scene_->Initialize(last_param_.get());
		}
	}

	if (scene_) {
		scene_->Update(df);
	}
}

/*!
 * @brief �`��
 */
void SceneManager::Render()
{
	if (scene_) {
		scene_->Render();
	}
}
