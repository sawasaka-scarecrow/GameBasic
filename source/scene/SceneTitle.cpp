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

static const float PI2 = 6.28318530718f;

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
		//SoundManager::GetInstance()->PlayBgm(CRI_BGM_VILLAGE);
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

	// ���f���X�V
	model_.Update(df);

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
		raster_scroll_.SetRate(2.0f, 1.0f, 0.05f);
		raster_scroll_.Start(3.0f, 1.0f);
	}
	raster_scroll_.Update(df);
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
	render_target_.ApplyRenderTarget();
	Render2D_();
	render_target_.ResetRenderTarget();

	raster_scroll_.Render(&render_target_);
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

	model_.LoadFile("data/model/ch00_stand00.x");
	
	model_.SetPotision(6.0f, 0, 0);
	model_.SetRotate(0, 0.075f, 0);

	render_target_.CreateRenderTarget(TEXTURE_FORMAT_ARGB8888, DeviceManager::GetInstance()->GetWidth(), DeviceManager::GetInstance()->GetHeight());
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
		} else if (KeyManager::GetInstance()->IsPress('Q')) {
			auto rotate = model_.GetRotate();
			rotate.x += 0.01f;
			model_.SetRotate(rotate);
		} else if (KeyManager::GetInstance()->IsPress('W')) {
			auto rotate = model_.GetRotate();
			rotate.y += 0.01f;
			model_.SetRotate(rotate);
		} else if (KeyManager::GetInstance()->IsPress('E')) {
			auto rotate = model_.GetRotate();
			rotate.z += 0.01f;
			model_.SetRotate(rotate);
		} else if (KeyManager::GetInstance()->IsPress('Z')) {
			model_.SetRotate(0, 0, 0);
		} else if (KeyManager::GetInstance()->IsPress('B')) {
			auto color = model_.GetColor();
			color.SetA(128);
			model_.SetColor(color);
		} else if (KeyManager::GetInstance()->IsPress('V')) {
			auto color = model_.GetColor();
			color.SetA(255);
			model_.SetColor(color);
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

void SceneTitle::Render2D_()
{
	// �}�X�^�[�f�[�^�Ɋ�Â��`�揈��
	Utility::BasicRender(MasterData::TitleImageList, ui_, textures_);
	Renderer::GetInstance()->PushState();
	Renderer::GetInstance()->SetZEnable(true);
	model_.Render();
	Renderer::GetInstance()->PopState();
}
