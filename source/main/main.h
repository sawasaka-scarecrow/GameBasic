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

private :
	void Update_(float df);
	void Render_();
};
