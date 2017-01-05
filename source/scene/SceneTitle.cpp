#include "SceneTitle.h"
#include "utility/Renderer.h"
#include "utility/Utility.hpp"
#include "utility/DeviceManager.h"
#include "utility/FadeManager.h"
#include "utility/KeyManager.h"
#include "utility/SoundManager.h"
#include "sound/Bgm.h"
#include "sound/Se.h"

/*!
 * @brief �X�V
 */
void SceneTitle::Initialize(const SceneBaseParam* param)
{
	// �}�X�^�[�f�[�^�ǂݍ���
	Reload_();
	
	// �e��J�E���^�[������
	stop_counter_.set(0, DeviceManager::GetInstance()->GetWidth() - 150, 6);
	root_counter_.set(0, DeviceManager::GetInstance()->GetWidth(), 4);
	cursor_counter_.set(0, 10, -3);
	SetBezier_(Bezier::Linear);

	state_.Change(ST_INIT);
}

/*!
 * @brief �X�V
 */
void SceneTitle::Update(float df)
{
	state_.Check(df);
	// ������
	if (state_ == ST_INIT) {
		state_.Change(ST_FADE_IN_INIT, true);
	}

	// �t�F�[�h�C��
	if (state_ == ST_FADE_IN_INIT) {
		FadeManager::GetInstance()->FadeIn();
		state_.Change(ST_FADE_IN, true);
	}
	if (state_ == ST_FADE_IN) {
		if (FadeManager::GetInstance()->IsEnd()) {
			state_.Change(ST_UPDATE);
		}
	}

	if (GetKeyState('S') < 0) {
		// �~�܂����J�E���^�[���ēx�Đ�
		stop_counter_.set(0, stop_counter_.getEnd(), 6);
	} else if (KeyManager::GetInstance()->IsTrg('1')) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_VILLAGE);
	} else if (KeyManager::GetInstance()->IsTrg('2')) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_FIELD);
	} else if (KeyManager::GetInstance()->IsTrg('3')) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_BATTLE);
	} else if (KeyManager::GetInstance()->IsTrg('4')) {
		SoundManager::GetInstance()->PlaySe(CRI_SE_OK, 0);
	} else if (KeyManager::GetInstance()->IsTrg('5')) {
		SoundManager::GetInstance()->PlaySe(CRI_SE_CANCEL, 0);
	} else if (KeyManager::GetInstance()->IsTrg('6')) {
		SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
	}

	++stop_counter_;
	++root_counter_;
	++cursor_counter_;
	++bezier_counter_;
	bezier_timer_.Update(df);

	// �Ƃ܂�`�𑀍�
	objects_["TextStop"]->x = stop_counter_;

	// �܂��`�𑀍�
	objects_["TextLoop"]->x = root_counter_;

	// �J�[�\���𑀍�
	objects_["TextCursor"]->y = cursour_y_ + cursor_counter_;

	// �x�W�F�J�E���^�[�𑀍�
	objects_["BezierCounter"]->x = bezier_counter_;

	// �J�[�\���𑀍�
	objects_["BezierTimer"]->x = bezier_timer_;
}

/*!
 * @brief �`��
 */
void SceneTitle::Render()
{
	// �}�X�^�[�f�[�^�Ɋ�Â��`�揈��
	Utility::BasicRender(MasterData::TitleImageList, MasterData::TitleUI, bitmaps_);
}

/*!
 * @brief �}�X�^�[�ēǂݍ���
 */
void SceneTitle::Reload_()
{
	// �}�X�^�[�f�[�^���ēǂݍ���
	Utility::ReloadMasterData();

	// FPS�̐ݒ�
	DeviceManager::GetInstance()->SetFPS(MasterData::Const.FPS);

	// �摜��ǂݍ���
	bitmaps_ = Utility::CreateBitmaps(MasterData::TitleImageList);

	// ���삵�₷���悤�ɃI�u�W�F�N�g������
	objects_ = Utility::CreateObjects<MasterData::TitleUIData>(MasterData::TitleUI);

	cursour_y_ = objects_["TextCursor"]->y;
}

void SceneTitle::SetBezier_(const Bezier::ControlPoint& cp)
{
	bezier_counter_.Set(100, 400, 60, cp);
	bezier_timer_.Set(100, 400, 1.0f, cp);
}
