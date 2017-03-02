#include "SceneOption.h"
#include "render/Renderer.h"
#include "utility/Utility.hpp"
#include "utility/FadeManager.h"
#include "utility/KeyManager.h"
#include "utility/SceneManager.h"
#include "sound/SoundManager.h"
#include "sound/Se.h"

static const int MaxVolume = 10;

/*!
 * @brief �X�V
 */
void SceneOption::Initialize(const SceneBaseParam* param)
{
	state_.Change(ST_INIT);
}

/*!
 * @brief �X�V
 */
void SceneOption::Update(float df)
{
	state_.Update(df);

	// ������
	if (state_ == ST_INIT) {
		state_.Change(ST_LOADING, true);
	}
	if (state_ == ST_LOADING) {
		Reload_();
		state_.Change(ST_IN_ANIMATION_INIT);
	}

	// �C���A�j���[�V����
	if (state_.IsRange(ST_IN_ANIMATION_INIT, ST_IN_ANIMATION)) {
		if (ActionInAnimation_(df)) {
			state_.Change(ST_SELECT_INIT);
		}
	}

	// �I��
	if (state_.IsRange(ST_SELECT_INIT, ST_SELECT)) {
		if (ActionSelect_(df)) {
			state_.Change(ST_OUT_ANIMATION_INIT);
		}
	}

	// �A�E�g�A�j���[�V����
	if (state_.IsRange(ST_OUT_ANIMATION_INIT, ST_OUT_ANIMATION)) {
		if (ActionOutAnimation_(df)) {
			state_.Change(ST_SELECT_INIT);
		}
	}

	// �J�[�\���X�V
	UpdateCursor_();
}

/*!
 * @brief �`��
 */
void SceneOption::Render2D()
{
	// �}�X�^�[�f�[�^�Ɋ�Â��`�揈��
	Utility::BasicRender(MasterData::OptionImageList, ui_, textures_);
}

/*!
 * @brief �f�[�^�ēǂݍ���
 */
void SceneOption::Reload_()
{
	// �摜��ǂݍ���
	textures_ = Utility::CreateBitmaps(MasterData::OptionImageList);

	// UI���R�s�[���ĕێ�
	ui_ = MasterData::OptionUI;

	// ���삵�₷���悤�ɃI�u�W�F�N�g������
	objects_ = Utility::CreateObjects<MasterData::OptionUIData>(ui_);

	// �J�[�\���̏����ʒu��ێ�
	cursor_x_[BgmLeft] = objects_["BgmLeftArrow"]->x;
	cursor_x_[BgmRight] = objects_["BgmRightArrow"]->x;
	cursor_x_[SeLeft] = objects_["SeLeftArrow"]->x;
	cursor_x_[SeRight] = objects_["SeRightArrow"]->x;
	cursor_x_[VoiceLeft] = objects_["VoiceLeftArrow"]->x;
	cursor_x_[VoiceRight] = objects_["VoiceRightArrow"]->x;
	cursor_ = Bgm;
	SetCursor_();

	// ���ʂ��擾
	volume_[Bgm] = static_cast<int>(SoundManager::GetInstance()->GetVolumeBgm() * 10);
	volume_[Se] = static_cast<int>(SoundManager::GetInstance()->GetVolumeSe() * 10);
	volume_[Voice] = static_cast<int>(SoundManager::GetInstance()->GetVolumeVoice() * 10);
	UpdateSound_();
}

/*!
 * @brief �J�[�\���̐ݒ�
 */
void SceneOption::SetCursor_()
{
	bgm_cursor_move_.set(0, GetCursorMove(Bgm), GetCursorSpeed(Bgm));
	se_cursor_move_.set(0, GetCursorMove(Se), GetCursorSpeed(Se));
	voice_cursor_move_.set(0, GetCursorMove(Voice), GetCursorSpeed(Voice));
}

/*!
 * @brief �J�[�\���X�V
 */
void SceneOption::UpdateCursor_()
{
	// �I�����ڂ̐F�ύX
	Utility::SetObjectColor(objects_["BgmValue"], (cursor_ == Bgm) ? Gdiplus::Color::Red : Gdiplus::Color::White);
	Utility::SetObjectColor(objects_["SeValue"], (cursor_ == Se) ? Gdiplus::Color::Red : Gdiplus::Color::White);
	Utility::SetObjectColor(objects_["VoiceValue"], (cursor_ == Voice) ? Gdiplus::Color::Red : Gdiplus::Color::White);
	Utility::SetObjectColor(objects_["Exit"], (cursor_ == Exit) ? Gdiplus::Color::Red : Gdiplus::Color::White);

	// ���E���̈ړ�
	if (state_ == ST_SELECT) {
		bgm_cursor_move_++;
		se_cursor_move_++;
		voice_cursor_move_++;
		objects_["BgmLeftArrow"]->x = cursor_x_[BgmLeft] - bgm_cursor_move_;
		objects_["BgmRightArrow"]->x = cursor_x_[BgmRight] + bgm_cursor_move_;
		objects_["SeLeftArrow"]->x = cursor_x_[SeLeft] - se_cursor_move_;
		objects_["SeRightArrow"]->x = cursor_x_[SeRight] + se_cursor_move_;
		objects_["VoiceLeftArrow"]->x = cursor_x_[VoiceLeft] - voice_cursor_move_;
		objects_["VoiceRightArrow"]->x = cursor_x_[VoiceRight] + voice_cursor_move_;
	}
}

/*!
 * @brief �T�E���h�X�V
 */
void SceneOption::UpdateSound_()
{
	// ���ʂ̐ݒ�
	SoundManager::GetInstance()->SetVolumeBgm(volume_[Bgm] / 10.0f);
	SoundManager::GetInstance()->SetVolumeSe(volume_[Se] / 10.0f);
	SoundManager::GetInstance()->SetVolumeVoice(volume_[Voice] / 10.0f);

	// ���ʕ\���̐ݒ�
	objects_["BgmValue"]->str = Utility::StringFormat("%3d%%", volume_[Bgm] * 10);
	objects_["SeValue"]->str = Utility::StringFormat("%3d%%", volume_[Se] * 10);
	objects_["VoiceValue"]->str = Utility::StringFormat("%3d%%", volume_[Voice] * 10);

	// ���̕\���̗L��
	objects_["BgmLeftArrow"]->visible =  (volume_[Bgm] != 0);
	objects_["BgmRightArrow"]->visible = (volume_[Bgm] != MaxVolume);
	objects_["SeLeftArrow"]->visible = (volume_[Se] != 0);
	objects_["SeRightArrow"]->visible = (volume_[Se] != MaxVolume);
	objects_["VoiceLeftArrow"]->visible = (volume_[Voice] != 0);
	objects_["VoiceRightArrow"]->visible = (volume_[Voice] != MaxVolume);
}

/*!
 * @brief �J�[�\���ړ��ʎ擾
 */
int SceneOption::GetCursorMove(Menu menu) const
{
	return cursor_ == menu ? MasterData::OptionConst.CursorMove : 0;
}

/*!
 * @brief �J�[�\���ړ����x�擾
 */
int SceneOption::GetCursorSpeed(Menu menu) const
{
	return cursor_ == menu ? MasterData::OptionConst.CursorSpeed : 0;
}

/*!
 * @brief �C���A�j���[�V����
 */
bool SceneOption::ActionInAnimation_(float df)
{
	if (state_ == ST_IN_ANIMATION_INIT) {
		animtion_.SetIn(ui_, MasterData::OptionInOut);
		state_.Change(ST_IN_ANIMATION, true);
	}
	if (state_ == ST_IN_ANIMATION) {
		animtion_.Update(df);
		if (animtion_.IsEnd()) {
			return true;
		}
	}

	return false;
}

/*!
 * @brief �I��
 */
bool SceneOption::ActionSelect_(float df)
{
	if (state_ == ST_SELECT_INIT) {
		state_.Change(ST_SELECT, true);
	}
	if (state_ == ST_SELECT) {
		if (KeyManager::GetInstance()->IsTrg(VK_UP)) {
			// ��
			cursor_ = static_cast<Menu>((cursor_ + (MenuMax - 1)) % MenuMax);
			SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
			SetCursor_();
		} else if (KeyManager::GetInstance()->IsTrg(VK_DOWN)) {
			// ��
			cursor_ = static_cast<Menu>((cursor_ + 1) % MenuMax);
			SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
			SetCursor_();
		} else if (KeyManager::GetInstance()->IsRepeat(VK_LEFT) && cursor_ != Exit) {
			// ��
			if (volume_[cursor_] > 0) {
				SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
				volume_[cursor_]--;
				UpdateSound_();
			}
		} else if (KeyManager::GetInstance()->IsRepeat(VK_RIGHT) && cursor_ != Exit) {
			// �E
			if (volume_[cursor_] < MaxVolume) {
				SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
				volume_[cursor_]++;
				UpdateSound_();
			}
		} else if (KeyManager::GetInstance()->IsTrg(VK_RETURN) && cursor_ == Exit) {
			// �I��
			SoundManager::GetInstance()->PlaySe(CRI_SE_OK, 0);
			return true;
		}
	}

	return false;
}

/*!
 * @brief �A�E�g�A�j���[�V����
 */
bool SceneOption::ActionOutAnimation_(float df)
{
	if (state_ == ST_OUT_ANIMATION_INIT) {
		animtion_.SetOut(ui_, MasterData::OptionInOut);
		state_.Change(ST_OUT_ANIMATION, true);
	}
	if (state_ == ST_OUT_ANIMATION) {
		animtion_.Update(df);
		if (animtion_.IsEnd()) {
			SceneManager::GetInstance()->Back();
			return true;
		}
	}

	return false;
}
