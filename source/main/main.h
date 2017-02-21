#pragma once

#include "WinMain.h"

class MainApp : public AppBase
{
public :
	MainApp(TCHAR* wnClassName, TCHAR* title);
	~MainApp();

	/*!
	 * @brief �p�������E�C���h�v���V�[�W��
	 */
	virtual LRESULT WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

	/*!
	 * @brief �p������������
	 */
	virtual void Initialize();

	/*!
	 * @brief �p�������I����
	 */
	virtual void Finalize();

	/*!
	 * @brief �p���������C�����[�v
	 */
	virtual void Update(float dt);
	
	/*!
	 * @brief ��ʃN���A
	 */
	virtual void ClearScreen();

	/*!
	 * @brief 3D�����_�����O
	 */
	virtual void Render3D();

	/*!
	 * @brief 2D�����_�����O
	 */
	virtual void Render2D();

private :
	void Update_(float df);
	void RenderDebug_();

	bool is_debug_render_;
};
