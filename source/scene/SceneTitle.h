#pragma once

#include "main/WinMain.h"
#include "SceneBase.h"
#include "utility/counter.h"
#include "utility/bezier.h"
#include "master/MasterData.hpp"
#include <memory>
#include <map>

/*!
 * @brief �^�C�g���V�[��
 */
class SceneTitle : public SceneBase
{
public :
	~SceneTitle(){}

	/*!
	 * @brief ������
	 */
	void Initialize(const SceneBaseParam* param);

	/*!
	 * @brief �X�V
	 *
	 * @param df �O�̃t���[���Ƃ̍�������
	 */
	void Update(float df);

	/*!
	 * @brief �`��
	 */
	void Render();

private :
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
