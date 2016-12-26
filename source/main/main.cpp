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
	MainApp app(_T("GameBasic"), _T("�Q�[���J���̃x�[�X"));

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
	} else if (msg == WM_ACTIVATE && wParam == 0) {
		KeyManager::GetInstance()->Clear();
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

	// �f�o�b�O�@�\
	if (KeyManager::GetInstance()->IsTrg('P')) {
		// ���݂̃V�[���̍ēǂݍ���
		SceneManager::GetInstance()->Restart();
	} else if (KeyManager::GetInstance()->IsTrg('O')) {
		// �f�o�b�O�o�̓E�C���h�E���J��
		ConsoleManager::GetInstance()->Open();
	} else if (KeyManager::GetInstance()->IsTrg('C')) {
		// �f�o�b�O�o�̓E�C���h�E�����
		ConsoleManager::GetInstance()->Close();
	}

	// �X�V
	Update_(df);

	// �`��
	Render_();
}

void MainApp::Update_(float df)
{
	// �t�F�[�h�̍X�V
	FadeManager::GetInstance()->Update(df);

	// �V�[���̍X�V
	SceneManager::GetInstance()->Update(df);
}

void MainApp::Render_()
{
	// ��ʃN���A
	Renderer::GetInstance()->ClearScreen(Gdiplus::Color(0, 80, 255));

	// �V�[��
	SceneManager::GetInstance()->Render();

	// �t�F�[�h
	FadeManager::GetInstance()->Render();

	// FPS�\��
	Renderer::GetInstance()->DrawStringFormat(Renderer::RIGHT_TOP, GetWidth(), 0, 16, Gdiplus::Color::White, _T("FPS:%.1f"), GetAverageFPS());
}
