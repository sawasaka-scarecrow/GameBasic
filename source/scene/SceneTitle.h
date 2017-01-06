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
		Start,
		Option,
		Exit,

		MenuMax,
	};

	void Reload_();
	void CheckCursor_();

	// ����
	bool ActionInAnimation_(float df);		// �C���A�j���[�V����
	bool ActionSelect_(float df);			// �I��
	bool ActionOutAnimation_(float df);		// �A�E�g�A�j���[�V����


	std::map<std::string, std::shared_ptr<Gdiplus::Bitmap>> bitmaps_;				// �摜�f�[�^
	std::map<std::string, MasterData::TitleUIData*> objects_;						// �\����
	InOutAnimation<MasterData::TitleUIData, MasterData::TitleInOutData> animtion_;	// InOut�A�j���[�V����
	StateManager<State> state_;														// ��ԊǗ�

	Bezier::Timer<int> bezier_timer_;

	Menu cursor_;
};
