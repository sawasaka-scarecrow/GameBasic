#include "SceneInitialize.h"
#include "utility/SceneManager.h"
#include "utility/SoundManager.h"
#include "utility/Renderer.h"

/*!
 * @brief �X�V
 */
void SceneInitialize::Update(float df)
{
	// �T�E���h������
	SoundManager::InitializeParam param {
		1,				//  BGM�̍ő哯���Đ���
		8,				//  SE�̍ő哯���Đ���
		4,				//  VOICE�̍ő哯���Đ���
		48000 * 2,		//  �ő�T���v�����O���[�g
		60,				//  1FPS
		"data/sound/GameBasic.acf"		//  AFC�t�@�C���̃p�X
	};
	SoundManager::GetInstance()->Initialize(param);
	SoundManager::GetInstance()->LoadBgmFile("data/sound/Bgm.acb", "data/sound/Bgm.awb");	// BGM�ǂݍ���
	SoundManager::GetInstance()->LoadSeFile("data/sound/Se.acb");							// SE�ǂݍ���

	// �^�C�g���֑J��
	SceneManager::GetInstance()->Change(SceneList::Title, nullptr);
}

/*!
 * @brief �`��
 */
void SceneInitialize::Render()
{
}
