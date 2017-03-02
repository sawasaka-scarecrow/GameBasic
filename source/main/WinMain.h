#pragma once

#include <Windows.h>
#include <tchar.h>
#include <vector>
#include <memory>
#include "Device.h"

class AppBase
{
public :
	AppBase(TCHAR* wnClassName, TCHAR* title);
	virtual ~AppBase();

	/*!
	 * @brief �E�C���h�E�̍��W��ύX
	 */
	void SetWindowPosition(int x, int y);

	/*!
	 * @brief �E�C���h�E�̃T�C�Y��ύX
	 */
	void SetWindowSize(int w, int h);

	/*!
	 * @brief �E�C���h�E��X���W�擾
	 */
	int GetPostionX() const { return wnd_x_; }

	/*!
	 * @brief �E�C���h�E��Y���W�擾
	 */
	int GetPostionY() const { return wnd_y_; }

	/*!
	 * @brief �E�C���h�E�̕��擾
	 */
	int GetWidth() const { return wnd_w_; }

	/*!
	 * @brief �E�C���h�E�̍����擾
	 */
	int GetHeight() const { return wnd_h_; }

	/*!
	 * @brief �E�C���h�E�̃n���h���擾
	 */
	HWND GetWindowHandle() const { return hwnd_; }

	/*!
	 * @brief FPS�̐ݒ�
	 */
	void SetFPS(int fps) { fps_ = fps; }

	/*!
	 * @brief FPS�̎擾
	 */
	int GetFPS() const { return fps_; }

	/*!
	 * @brief ����FPS�̎擾
	 */
	float GetAverageFPS() const { return average_fps_; }
	
	/*!
	 * @brief �f�o�C�X�̎擾
	 */
	Device& GetDevice() { return device_; }

	/*!
	 * @brief �J�n
	 */
	bool Start();

public :
	/*!
	 * @brief �E�C���h�v���V�[�W���̉��z�֐�
	 */
	virtual LRESULT WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam) = 0;

	/*!
	 * @brief ������
	 */
	virtual void Initialize() = 0;

	/*!
	 * @brief �I����
	 */
	virtual void Finalize() = 0;

	/*!
	 * @brief ���C�����[�v
	 */
	virtual void Update(float df) = 0;

	/*!
	 * @brief ��ʃN���A
	 */
	virtual void ClearScreen(){}

	/*!
	 * @brief 3D�����_�����O
	 */
	virtual void Render3D(){}

	/*!
	 * @brief 2D�����_�����O
	 */
	virtual void Render2D(){}

private :
	static LRESULT CALLBACK WndProc_(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

	/*!
	 * @brief �N���C�A���g�̈�̃T�C�Y���擾
	 */
	SIZE GetClientSize_(int w, int h);

	/*!
	 * @brief �������@�҂�
	 */
	float VSync_();

	/*!
	 * @brief �`�揈��
	 */
	void Render_();

	Device device_;
	TCHAR wndClassName_[256];
	TCHAR title_[256];
	TIMECAPS timer_caps_;
	WNDCLASSEX wcex_;
	int wnd_x_;
	int wnd_y_;
	int wnd_w_;
	int wnd_h_;
	float average_fps_;
	int fps_;
	DWORD fps_count_;
	DWORD old_time_;
	DWORD start_time_;
	HWND hwnd_;
	DWORD style_;
	bool active_;
};
