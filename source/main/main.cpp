#include "main.h"
#include "utility/SceneManager.h"
#include "utility/Renderer.h"
#include "utility/DeviceManager.h"
#include "utility/FadeManager.h"
#include "utility/ConsoleManager.h"
#include "utility/KeyManager.h"
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
	if (msg == WM_KEYDOWN) {
		KeyManager::GetInstance()->Down(wParam);
	} else if (msg == WM_KEYUP) {
		KeyManager::GetInstance()->Up(wParam);
	}

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

	ConsoleManager::GetInstance()->Open();
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
	// �L�[�̍X�V
	KeyManager::GetInstance()->Update();

	// �V�[���̍ēǂݍ���
	if (KeyManager::GetInstance()->IsTrg('P')) {
		SceneManager::GetInstance()->Restart();
	}

	// �t�F�[�h�̍X�V
	FadeManager::GetInstance()->Update(df);

	// �V�[���̍X�V
	SceneManager::GetInstance()->Update(df);

	// �t�F�[�h����
	FadeManager::GetInstance()->Render();

	// FPS�\��
	Renderer::GetInstance()->DrawStringFormat(Renderer::RIGHT_TOP, GetWidth(), 0, 16, Gdiplus::Color::White, _T("FPS:%.1f"), GetAverageFPS());
}
