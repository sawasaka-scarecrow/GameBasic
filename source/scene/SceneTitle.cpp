#include "SceneTitle.h"
#include "render/Renderer.h"
#include "utility/Utility.hpp"
#include "utility/FadeManager.h"
#include "utility/KeyManager.h"
#include "utility/SceneManager.h"
#include "sound/SoundManager.h"
#include "sound/Bgm.h"
#include "sound/Se.h"
#include "scene/SceneGame.h"
#include <algorithm>

/*!
 * @brief �X�V
 */
void SceneTitle::Initialize(const SceneBaseParam* param)
{
	// �}�X�^�[�f�[�^�ǂݍ���
	cursor_ = Start;

	state_.Change(ST_INIT);
}

/*!
 * @brief �X�V
 */
void SceneTitle::Update(float df)
{
	state_.Update(df);

	// ������
	if (state_ == ST_INIT) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_VILLAGE);
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

	if (KeyManager::GetInstance()->IsTrg('1')) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_VILLAGE);
	} else if (KeyManager::GetInstance()->IsTrg('2')) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_FIELD);
	} else if (KeyManager::GetInstance()->IsTrg('3')) {
		SoundManager::GetInstance()->PlayBgm(CRI_BGM_BATTLE);
	} else if (KeyManager::GetInstance()->IsTrg('4')) {
		SoundManager::GetInstance()->PlaySe(CRI_SE_OK);
	} else if (KeyManager::GetInstance()->IsTrg('5')) {
		SoundManager::GetInstance()->PlaySe(CRI_SE_CANCEL, 0);
	} else if (KeyManager::GetInstance()->IsTrg('6')) {
		SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
	} else if (KeyManager::GetInstance()->IsTrg('9')) {
		SoundManager::GetInstance()->StopAll();
	}
}

/*!
 * @brief 3D�`��
 */
void SceneTitle::Render3D()
{
}

/*!
 * @brief �`��
 */
void SceneTitle::Render2D()
{
	// �}�X�^�[�f�[�^�Ɋ�Â��`�揈��
	Utility::BasicRender(MasterData::TitleImageList, ui_, textures_);
}

/*!
 * @brief �f�[�^�ēǂݍ���
 */
void SceneTitle::Reload_()
{
	// �摜��ǂݍ���
	textures_ = Utility::CreateBitmaps(MasterData::TitleImageList);

	// UI���R�s�[���ĕێ�
	ui_ = MasterData::TitleUI;

	// ���삵�₷���悤�ɃI�u�W�F�N�g������
	objects_ = Utility::CreateObjects<MasterData::TitleUIData>(ui_);
}

/*!
 * @brief �J�[�\���X�V
 */
void SceneTitle::UpdateCursor_()
{
	static const char* button[] = { "ButtonGray", "ButtonRed" };

	objects_["StartButton"]->str = button[cursor_ == Start ? 1 : 0];
	objects_["OptionButton"]->str = button[cursor_ == Option ? 1 : 0];
	objects_["ExitButton"]->str = button[cursor_ == Exit ? 1 : 0];
}

/*!
 * @brief �C���A�j���[�V����
 */
bool SceneTitle::ActionInAnimation_(float df)
{
	if (state_ == ST_IN_ANIMATION_INIT) {
		animtion_.SetIn(ui_, MasterData::TitleInOut);
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
bool SceneTitle::ActionSelect_(float df)
{
	if (state_ == ST_SELECT_INIT) {
		state_.Change(ST_SELECT, true);
	}
	if (state_ == ST_SELECT) {
		if (KeyManager::GetInstance()->IsTrg(VK_UP)) {
			// ��
			cursor_ = static_cast<Menu>((cursor_ + (MenuMax - 1)) % MenuMax);
			SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
		} else if (KeyManager::GetInstance()->IsTrg(VK_DOWN)) {
			// ��
			cursor_ = static_cast<Menu>((cursor_ + 1) % MenuMax);
			SoundManager::GetInstance()->PlaySe(CRI_SE_CURSOR, 0);
		} else if (KeyManager::GetInstance()->IsTrg(VK_RETURN)) {
			// ����
			SoundManager::GetInstance()->PlaySe(CRI_SE_OK, 0);
			return true;
		}
	}

	return false;
}

/*!
 * @brief �A�E�g�A�j���[�V����
 */
bool SceneTitle::ActionOutAnimation_(float df)
{
	if (state_ == ST_OUT_ANIMATION_INIT) {
		animtion_.SetOut(ui_, MasterData::TitleInOut);
		state_.Change(ST_OUT_ANIMATION, true);
	}
	if (state_ == ST_OUT_ANIMATION) {
		animtion_.Update(df);
		if (animtion_.IsEnd()) {
			if (cursor_ == Start) {
				std::shared_ptr<SceneGameParam> param(new SceneGameParam());
				param->str = "����������";
				SceneManager::GetInstance()->Change(SceneList::Game, param);
			} else if (cursor_ == Option) {
				SceneManager::GetInstance()->Change(SceneList::Option, nullptr);
			} else if (cursor_ == Exit) {
				DeviceManager::GetInstance()->Exit();
			}
			return true;
		}
	}

	return false;
}
