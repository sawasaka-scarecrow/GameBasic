#include "SceneManager.h"
#include "scene/SceneBase.h"
#include "scene/SceneInitialize.h"
#include "scene/SceneTitle.h"

/*!
 * @brief �V�[���؂�ւ�
 */
void SceneManager::Change(SceneList::List scene, std::shared_ptr<SceneBaseParam> param)
{
	scene_state_.Change(scene);
	param_ = param;
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
	if (scene_state_.Check(df) || is_restart_) {
		is_restart_ = false;
		if (scene_) {
			scene_->Finalize();
		}
		switch (scene_state_) {
		case SceneList::Initialize :	scene_.reset(new SceneInitialize());	break;
		case SceneList::Title :			scene_.reset(new SceneTitle());			break;
		default : break;
		}
		if (scene_) {
			scene_->Initialize(param_.get());
		}
	}

	if (scene_) {
		scene_->Update(df);
		scene_->Render();
	}
}
