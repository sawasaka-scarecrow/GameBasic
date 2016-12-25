#include "main.h"
#include "utility/SceneManager.h"
#include "utility/Renderer.h"
#include "utility/DeviceManager.h"
#include <set>
#include <algorithm>

/*!
 * @brief WinMain
 */
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// �N���X���ƃ^�C�g����ݒ�
	MainApp app(_T("bezier"), _T("bezier�̃e�X�g"));

	// �A�v�����X�^�[�g
	app.Start();

	return 0;
}

MainApp::MainApp(TCHAR* wnClassName, TCHAR* title)
	: AppBase(wnClassName, title)
{
}

MainApp::~MainApp()
{
}

/*!
 * @brief �p�������E�C���h�v���V�[�W��
 */
LRESULT MainApp::WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*!
 * @brief �p������������
 */
void MainApp::Initialize()
{
	// �����_���[��������
	Renderer::GetInstance()->Initialize(GetBackHDC());

	// �f�o�C�X�}�l�[�W���[�̏�����
	DeviceManager::GetInstance()->Initialize(this);

	// �V�[���؂�ւ�
	SceneManager::GetInstance()->Change(SceneList::Initialize, nullptr);
}

/*!
 * @brief �p�������I����
 */
void MainApp::Finalize()
{
}

/*!
 * @brief �p���������C�����[�v
 */
void MainApp::Update(float df)
{
	// �V�[���̍ēǂݍ���
	if (GetKeyState('P') < 0) {
		SceneManager::GetInstance()->Restart();
	}

	// �V�[���̍X�V
	SceneManager::GetInstance()->Update(df);

	// FPS�\��
	Renderer::GetInstance()->DrawStringFormat(Renderer::RIGHT_TOP, GetWidth(), 0, 16, Gdiplus::Color::White, _T("FPS:%.1f"), GetAverageFPS());
}
