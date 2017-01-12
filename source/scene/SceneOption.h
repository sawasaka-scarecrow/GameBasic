#pragma once

#include "SceneBase.h"
#include "utility/counter.h"
#include "utility/StateManager.h"
#include "utility/InOutAnimation.h"
#include "master/MasterData.hpp"
#include <memory>
#include <map>

/*!
 * @brief �I�v�V�����V�[��
 */
class SceneOption : public SceneBase
{
public :
	~SceneOption(){}

	/*!
	 * @brief ������
	 */
	void Initialize(const SceneBaseParam* param);

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

private :
	enum State {
		ST_INIT,
		ST_LOADING,

		// �C���A�j���[�V����
		ST_IN_ANIMATION_INIT,
		ST_IN_ANIMATION,

		// �X�V
		ST_SELECT_INIT,
		ST_SELECT,

		// �A�E�g�A�j���[�V����
		ST_OUT_ANIMATION_INIT,
		ST_OUT_ANIMATION,

		ST_EXIT,
	};
	
	enum Menu {
		Bgm,
		Se,
		Voice,
		Exit,

		MenuMax,
	};

	enum Cursor {
		BgmLeft,
		BgmRight,
		SeLeft,
		SeRight,
		VoiceLeft,
		VoiceRight,

		CursorMax,
	};

	void Reload_();							// �f�[�^�ēǂݍ���
	void SetCursor_();						// �J�[�\���̐ݒ�
	void UpdateCursor_();					// �J�[�\���X�V
	void UpdateSound_();					// �T�E���h�X�V
	int GetCursorMove(Menu menu) const;		// �J�[�\���ړ��ʎ擾
	int GetCursorSpeed(Menu menu) const;	// �J�[�\���ړ����x�擾

	// ����
	bool ActionInAnimation_(float df);		// �C���A�j���[�V����
	bool ActionSelect_(float df);			// �I��
	bool ActionOutAnimation_(float df);		// �A�E�g�A�j���[�V����

	std::map<std::string, std::shared_ptr<Gdiplus::Bitmap>> bitmaps_;					// �摜�f�[�^
	std::vector<MasterData::OptionUIData> ui_;											// UI
	std::map<std::string, MasterData::OptionUIData*> objects_;							// �I�u�W�F�N�g
	InOutAnimation<MasterData::OptionUIData, MasterData::OptionInOutData> animtion_;	// InOut�A�j���[�V����
	StateManager<State> state_;															// ��ԊǗ�

	Menu cursor_;
	LoopCounter bgm_cursor_move_;
	LoopCounter se_cursor_move_;
	LoopCounter voice_cursor_move_;
	int cursor_x_[CursorMax];
	int volume_[CursorMax - 1];
};
