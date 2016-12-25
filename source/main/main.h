#pragma once

#include <memory>
#include <map>
#include "WinMain.h"
#include "utility/counter.h"
#include "utility/bezier.h"
#include "master/MasterData.hpp"

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
	/*!
	 * @brief �}�X�^�[�ēǂݍ���
	 */
	void Reload_();

	void SetBezier_(const Bezier::ControlPoint& cp);


	std::map<std::string, std::shared_ptr<Gdiplus::Bitmap>> bitmaps_;
	std::map<std::string, MasterData::TitleUIData*> objects_;

	SaturationCounter stop_counter_;
	RootCounter root_counter_;
	InteriorCounter fade_counter_;
	LoopCounter cursor_counter_;
	Bezier::Counter<int> bezier_counter_;
	Bezier::Timer<int> bezier_timer_;

	int cursour_y_;
};
