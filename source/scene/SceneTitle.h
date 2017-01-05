#pragma once

#include "main/WinMain.h"
#include "SceneBase.h"
#include "utility/counter.h"
#include "utility/bezier.h"
#include "utility/StateManager.h"
#include "utility/InOutAnimation.h"
#include "master/MasterData.hpp"
#include <memory>
#include <map>

/*!
 * @brief �^�C�g���V�[��
 */
class SceneTitle : public SceneBase
{
public :
	~SceneTitle(){}

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

		// �t�F�[�h�C��
		ST_FADE_IN_INIT,
		ST_FADE_IN,

		// �X�V
		ST_SELECT_INIT,
		ST_SELECT,

		// �t�F�[�h�A�E�g
		ST_FADE_OUT_INIT,
		ST_FADE_OUT,

		ST_EXIT,
	};
	
	void Reload_();
	void CheckCursor_();

	// ����
	bool ActionFadeIn_(float df);			// �t�F�[�h�C��
	bool ActionSelect_(float df);			// �I��
	bool ActionFadeOut_(float df);			// �t�F�[�h�A�E�g


	std::map<std::string, std::shared_ptr<Gdiplus::Bitmap>> bitmaps_;				// �摜�f�[�^
	std::map<std::string, MasterData::TitleUIData*> objects_;						// �\����
	InOutAnimation<MasterData::TitleUIData, MasterData::TitleInOutData> animtion_;	// InOut�A�j���[�V����
	StateManager<State> state_;														// ��ԊǗ�

	Bezier::Timer<int> bezier_timer_;

	int cursor_;
};
