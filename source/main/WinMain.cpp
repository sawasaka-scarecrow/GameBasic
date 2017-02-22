#include "WinMain.h"
#include <stdarg.h>

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Shlwapi.lib")

AppBase::AppBase(TCHAR* wnClassName, TCHAR* title)
	: wnd_x_(32)
	, wnd_y_(32)
	, wnd_w_(800)
	, wnd_h_(450)
	, average_fps_(0)
	, fps_(60)
	, fps_count_(0)
	, old_time_(0)
	, hwnd_(NULL)
	, style_(WS_OVERLAPPED |  WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
{
	// �^�C�}�[���x�̏�����
	timeGetDevCaps(&timer_caps_, sizeof(TIMECAPS));
	timeBeginPeriod(timer_caps_.wPeriodMin);

	lstrcpy(wndClassName_, wnClassName);
	lstrcpy(title_, title);

	// �E�C���h�E�N���X�̏�����
	wcex_.cbSize = sizeof(WNDCLASSEX);
	wcex_.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex_.lpfnWndProc = (WNDPROC)WndProc_;
	wcex_.cbClsExtra = 0;
	wcex_.cbWndExtra = 0;
	wcex_.hInstance = GetModuleHandle(NULL);
	wcex_.hIcon = LoadIcon(NULL , IDI_APPLICATION);
	wcex_.hCursor = LoadCursor(NULL , IDC_ARROW) ;
	wcex_.hIconSm = LoadIcon(NULL , IDI_APPLICATION);
	wcex_.lpszMenuName = NULL;
	wcex_.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex_.lpszClassName = wndClassName_;
}

AppBase::~AppBase()
{
	timeEndPeriod(timer_caps_.wPeriodMin);
}

/*!
 * @brief �E�C���h�E�̍��W��ύX
 */
void AppBase::SetWindowPosition(int x, int y)
{
	wnd_x_ = x;
	wnd_y_ = y;
	if (hwnd_ != NULL) {
		MoveWindow(hwnd_, wnd_x_, wnd_y_, wnd_w_, wnd_h_, TRUE);
	}
}

/*!
 * @brief �E�C���h�E�̃T�C�Y��ύX
 */
void AppBase::SetWindowSize(int w, int h)
{
	wnd_w_ = w;
	wnd_h_ = h;
	if (hwnd_ != NULL) {
		SIZE size = GetClientSize_(wnd_w_, wnd_h_);
		MoveWindow(hwnd_, wnd_x_, wnd_y_, size.cx, size.cy, TRUE);
	}
}

/*!
 * @brief �J�n
 */
bool AppBase::Start()
{
	// �E�C���h�E�o�^
	if (!RegisterClassEx(&wcex_)) {
		return false;
	}

	// �E�C���h�E����
	SIZE size = GetClientSize_(wnd_w_, wnd_h_);
	hwnd_ = ::CreateWindowEx(WS_EX_CLIENTEDGE,
		wndClassName_,
		title_,
		style_,
		wnd_x_,
		wnd_y_,
		size.cx,
		size.cy,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		this);
	if (hwnd_ == NULL) {
		return false;
	}
	ShowWindow(hwnd_, SW_SHOWNORMAL);
	UpdateWindow(hwnd_);

	// ���Ԃ̏�����
	old_time_ = timeGetTime();
	start_time_ = old_time_;

	// �f�o�C�X������
	device_.Initialize(hwnd_, wnd_w_, wnd_h_);

	// �p����̏������Ăяo��
	Initialize();

	// ���b�Z�[�W���[�v
	while (true) {
		MSG msg;
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			// �������@�҂�
			float df = VSync_();

			if (device_.CheckIdle()) {
				// �p����̃��C�����[�v�̌Ăяo��
				Update(df);

				// �`��
				Render_();
			}
		}
	}
	
	// �p����̏I�����Ăяo��
	Finalize();

	// �f�o�C�X�I��
	device_.Finalize();

	return true;
}

/*!
 * @brief ���C���E�C���h�E���b�Z�[�W
 */
LRESULT CALLBACK AppBase::WndProc_(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	AppBase* app = reinterpret_cast<AppBase*>(GetWindowLong(hWnd, GWL_USERDATA));
	switch (msg) {
	case WM_CREATE : {
			LPCREATESTRUCT data = reinterpret_cast< LPCREATESTRUCT >(lParam);
			app = reinterpret_cast<AppBase*>(data->lpCreateParams);
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast< long >(app));
			break;
		}
	case WM_DESTROY :
		::PostQuitMessage(0);
		return 0;
	case WM_SYSKEYDOWN :
		if (wParam == VK_F4) {
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		break;
	case WM_ACTIVATE :
		if (app != nullptr) {
			app->active_ = (wParam != 0);
			if (!app->active_) {
				app->fps_count_ = 0;
			}
		}
		break;
	// ���̓t�H�[�J�X�̎擾
	case WM_SETFOCUS :
		if (app != nullptr) {
			app->device_.CheckDevice();
		}
		break;
	}

	if (app != nullptr) {
		return app->WndProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*!
 * @brief �N���C�A���g�̈�̃T�C�Y���擾
 */
SIZE AppBase::GetClientSize_(int w, int h)
{
	RECT wndRect;
	wndRect.top = 0;
	wndRect.left = 0;
	wndRect.right = w;
	wndRect.bottom = h;
	AdjustWindowRectEx(&wndRect, style_, FALSE, WS_EX_CLIENTEDGE);

	// Window7���ƃN���C�A���g�̈�̈��������������̂Œ�������
	SIZE size = {
		wndRect.right - wndRect.left - 2,
		wndRect.bottom - wndRect.top - 2
	};
	return size;
}

/*!
 * @brief �������@�҂�
 */
float AppBase::VSync_()
{
	int fps = active_ ? fps_ : 10;

	// �҂����Ԕ���
	DWORD time = timeGetTime();
	int wait = ++fps_count_ * 1000 / fps - (time - start_time_);
	if (wait > 0) {
		Sleep(wait);
	} else {
		Sleep(0);
	}

	// �O�̃t���[���Ƃ̎��ԍ����擾
	time = timeGetTime();
	float ret = (time - old_time_) / 1000.0f;
	old_time_ = time;

	// ����FPS�v��
	if (fps_count_ % fps == 0) {
		average_fps_ = 1000.f / ((time - start_time_) / static_cast<float>(fps));
		fps_count_ = 0;
		start_time_ = time;
	}

	return ret;
}

/*!
 * @brief �`�揈��
 */
void AppBase::Render_()
{
	auto device = device_.GetDevice();

	ClearScreen();

	// 3D�`��
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
	device->BeginScene();
	Render3D();
	device->EndScene();

	// 2D�`��
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->BeginScene();
	Render2D();
	device->EndScene();

	device->Present(nullptr, nullptr, nullptr, nullptr);
}
