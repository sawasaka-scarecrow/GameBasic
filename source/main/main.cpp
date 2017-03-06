#include "main.h"
#include "render/Renderer.h"
#include "utility/SceneManager.h"
#include "utility/DeviceManager.h"
#include "utility/FadeManager.h"
#include "utility/ConsoleManager.h"
#include "utility/KeyManager.h"
#include "utility/MouseManager.h"
#include "utility/Utility.hpp"
#include "sound/SoundManager.h"
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
	, is_debug_render_(true)
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
		MouseManager::GetInstance()->Clear();
	} else if (msg == WM_SYSKEYDOWN) {
		if (wParam == VK_RETURN) {
		}
	}
	MouseManager::GetInstance()->WndProc(hWnd, msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*!
 * @brief �p������������
 */
void MainApp::Initialize()
{
	// �����_���[��������
	Renderer::GetInstance()->Initialize(this);

	// �f�o�C�X�}�l�[�W���[�̏�����
	DeviceManager::GetInstance()->Initialize(this);

	// �}�X�^�[�f�[�^�ǂݍ���
	ReloadMasterData_();

	// �V�[���؂�ւ�
	SceneManager::GetInstance()->Change(SceneList::Initialize, nullptr);

}

/*!
 * @brief �p�������I����
 */
void MainApp::Finalize()
{
	// �V�[���̏I��
	SceneManager::GetInstance()->Finalize();

	// �����_���[���I����
	Renderer::GetInstance()->Finalize();
}

/*!
 * @brief �p���������C�����[�v
 */
void MainApp::Update(float df)
{
	// �L�[�̍X�V
	KeyManager::GetInstance()->Update();

	// �}�E�X�X�V
	MouseManager::GetInstance()->Update();

	// �J�����̍X�V
	camera_.Update(df);

	// �f�o�b�O�@�\
	if (KeyManager::GetInstance()->IsTrg('R')) {
		// ���݂̃V�[���̍ēǂݍ���
		SoundManager::GetInstance()->StopAll();
		ReloadMasterData_();
		SceneManager::GetInstance()->Restart();
	} else if (KeyManager::GetInstance()->IsTrg('O')) {
		// �f�o�b�O�o�̓E�C���h�E���J��
		ConsoleManager::GetInstance()->Open();
	} else if (KeyManager::GetInstance()->IsTrg('C')) {
		// �f�o�b�O�o�̓E�C���h�E�����
		ConsoleManager::GetInstance()->Close();
	} else if (KeyManager::GetInstance()->IsTrg('Q')) {
		is_debug_render_ = !is_debug_render_;
	}

	// �X�V
	Update_(df);
}

void MainApp::Update_(float df)
{
	// �t�F�[�h�̍X�V
	FadeManager::GetInstance()->Update(df);

	// �V�[���̍X�V
	SceneManager::GetInstance()->Update(df);

	// �T�E���h�̍X�V
	SoundManager::GetInstance()->Update(df);
}

/*!
 * @brief ��ʃN���A
 */
void MainApp::ClearScreen()
{
	// ��ʃN���A
	Renderer::GetInstance()->ClearScreen(Color(255, 0, 255));
	//Renderer::GetInstance()->ClearScreen(Color(0, 0, 0));
}

/*!
 * @brief 3D�����_�����O
 */
void MainApp::Render3D()
{
	// �V�[��
	SceneManager::GetInstance()->Render3D();
}

/*!
 * @brief 2D�����_�����O
 */
void MainApp::Render2D()
{
	// �V�[��
	SceneManager::GetInstance()->Render2D();

	// �t�F�[�h
	FadeManager::GetInstance()->Render();

	// �f�o�b�O�\��
	RenderDebug_();

	// FPS�\��
	Renderer::GetInstance()->DrawStringFormat(Renderer::RIGHT_TOP, GetWidth(), 0, 12, Color::White, _T("FPS:%.1f"), GetAverageFPS());
}

void MainApp::RenderDebug_()
{
	if (!is_debug_render_) return;

	// �������
	const std::string ds[] = {
		camera_.GetInfo(),
		"R:�V�[�����Z�b�g",
		"O:�R���\�[�����J��",
		"C:�R���\�[�������",
		"Q:�f�o�b�O�̕\��/��\��",
	};
	for (int i = 0; i < sizeof(ds) / sizeof(*ds); ++i) {
		Renderer::GetInstance()->DrawString(ds[i].c_str(), Renderer::LEFT_TOP, 0, i * 12, 12);
	}
}

void MainApp::ReloadMasterData_()
{
	Utility::ReloadMasterData();
	DeviceManager::GetInstance()->SetFPS(MasterData::Const.FPS);
	SetWindowSize(MasterData::Const.window_width, MasterData::Const.window_height);
	GetDevice().SetBackBufferSize(MasterData::Const.backbuffer_width, MasterData::Const.backbuffer_height);
}
