#pragma once

#include "SceneBase.h"
#include "render/Texture.h"
#include "utility/StateManager.h"
#include "utility/InOutAnimation.h"
#include "master/MasterData.hpp"
#include "3d/SkinModel.h"
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
	 * @brief 3D�`��
	 */
	void Render3D();

	/*!
	 * @brief 2D�`��
	 */
	void Render2D();

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

	void Reload_();							// �f�[�^�ēǂݍ���
	void UpdateCursor_();					// �J�[�\���X�V

	// ����
	bool ActionInAnimation_(float df);		// �C���A�j���[�V����
	bool ActionSelect_(float df);			// �I��
	bool ActionOutAnimation_(float df);		// �A�E�g�A�j���[�V����


	std::map<std::string, std::shared_ptr<Texture>> textures_;						// �摜�f�[�^
	std::vector<MasterData::TitleUIData> ui_;										// UI
	std::map<std::string, MasterData::TitleUIData*> objects_;						// �I�u�W�F�N�g
	InOutAnimation<MasterData::TitleUIData, MasterData::TitleInOutData> animtion_;	// InOut�A�j���[�V����
	StateManager<State> state_;														// ��ԊǗ�

	//Model model_;
	SkinModel model_;
	Menu cursor_;	// �J�[�\��
};
