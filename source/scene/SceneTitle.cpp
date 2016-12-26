#include "SceneTitle.h"
#include "utility/Renderer.h"
#include "utility/Utility.hpp"
#include "utility/DeviceManager.h"
#include "utility/FadeManager.h"

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
	fade_counter_.set(0, 0, 1);
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

	// �t�F�[�h
	if (state_ == ST_FADE_IN_INIT) {
		FadeManager::GetInstance()->FadeIn();
		state_.Change(ST_FADE_IN, true);
	}
	if (state_ == ST_FADE_IN) {
		if (FadeManager::GetInstance()->IsEnd()) {
			state_.Change(ST_UPDATE, true);
		}
	}

	if (GetKeyState('R') < 0) {
		// �}�X�^�[�f�[�^�ēǂݍ���
		Reload_();
	} else if (GetKeyState('I') < 0) {
		// �t�F�[�h�C��
		fade_counter_.set(fade_counter_, 0, 30);
	} else if (GetKeyState('O') < 0) {
		// �t�F�[�h�A�E�g
		fade_counter_.set(fade_counter_, 255, 30);
	} else if (GetKeyState('S') < 0) {
		// �~�܂����J�E���^�[���ēx�Đ�
		stop_counter_.set(0, stop_counter_.getEnd(), 6);
	} else if (GetKeyState('1') < 0) {
		SetBezier_(Bezier::Linear);
	} else if (GetKeyState('2') < 0) {
		SetBezier_(Bezier::EaseIn);
	} else if (GetKeyState('3') < 0) {
		SetBezier_(Bezier::EaseOut);
	} else if (GetKeyState('4') < 0) {
		SetBezier_(Bezier::EaseInOut);
	} else if (GetKeyState('5') < 0) {
		SetBezier_(Bezier::EaseInBack);
	} else if (GetKeyState('6') < 0) {
		SetBezier_(Bezier::EaseOutBack);
	} else if (GetKeyState('7') < 0) {
		SetBezier_(Bezier::EaseInOutBack);
	} else if (GetKeyState('8') < 0) {
		SetBezier_(Bezier::EaseInCirc);
	} else if (GetKeyState('9') < 0) {
		SetBezier_(Bezier::ControlPoint(MasterData::Const.x1, MasterData::Const.y1, MasterData::Const.x2, MasterData::Const.y2));
	}

	++stop_counter_;
	++root_counter_;
	++fade_counter_;
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
	Utility::BaseRender(MasterData::TitleImageList, MasterData::TitleUI, bitmaps_);

	// �������
	static const char* ds[] = {
		"R:�}�X�^�[�ǂݍ���",
		"I:�t�F�[�h�C��",
		"O:�t�F�[�h�A�E�g",
		"S:�Ƃ܂�`���ēx�ړ�",
		"1:Bezier::Linear:",
		"2:Bezier::EaseIn:",
		"3:Bezier::EaseOut",
		"4:Bezier::EaseInOut",
		"5:Bezier::EaseInBack",
		"6:Bezier::EaseOutBack",
		"7:Bezier::EaseInOutBack",
		"8:Bezier::EaseInCirc",
		"9:Bezier �I���W�i��",
	};
	for (int i = 0; i < sizeof(ds) / sizeof(*ds); ++i) {
		Renderer::GetInstance()->DrawString(ds[i], Renderer::LEFT_TOP, 0, i * 16, 12);
	}
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

	// ���삵�₷���悤�Ƀf�[�^������
	objects_ = Utility::CreateObjects<MasterData::TitleUIData>(MasterData::TitleUI);

	cursour_y_ = objects_["TextCursor"]->y;
}

void SceneTitle::SetBezier_(const Bezier::ControlPoint& cp)
{
	bezier_counter_.Set(100, 400, 60, cp);
	bezier_timer_.Set(100, 400, 1.0f, cp);
}
