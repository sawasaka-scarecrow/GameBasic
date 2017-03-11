#include "SceneGame.h"
#include "render/Renderer.h"
#include "input/Key.h"
#include "utility/SceneManager.h"
#include "utility/ConsoleManager.h"

/*!
 * @brief ������
 */
void SceneGame::Initialize(const SceneBaseParam* param)
{
	param_ = *(static_cast<const SceneGameParam*>(param));
	ConsoleManager::GetInstance()->
		PrintLine(param_.str.c_str());
}

/*!
 * @brief �I����
 */
void SceneGame::Finalize()
{
}

/*!
 * @brief �X�V
 */
void SceneGame::Update(float df)
{
	if (Key::GetInstance()->IsTrg(KeyCode::A)) {
		SceneManager::GetInstance()->Back();
	}
}

/*!
 * @brief �`��
 */
void SceneGame::Render2D()
{
}
