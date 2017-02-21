#include "FadeManager.h"
#include "DeviceManager.h"
#include "ConsoleManager.h"
#include "render/Renderer.h"

/*!
 * @brief �t�F�[�h�C��
 */
void FadeManager::FadeIn(float time, const Color& color, float delay)
{
	alpha_.Set(255, 0, time, Bezier::Linear);
	delay_time_ = delay;
}

/*!
 * @brief �t�F�[�h�A�E�g
 */
void FadeManager::FadeOut(float time, const Color& color, float delay)
{
	alpha_.Set(0, 255, time, Bezier::Linear);
	delay_time_ = delay;
}

/*!
 * @brief �X�V
 */
void FadeManager::Update(float df)
{
	if (delay_time_ > 0) {
		delay_time_ -= df;
		//ConsoleManager::GetInstance()->Print("time:%9.3f\n", delay_time_);
		if (delay_time_ > 0) return;
	}
	alpha_.Update(df);
}

/*!
 * @brief �`��
 */
void FadeManager::Render()
{
	if (alpha_ == 0) return ;

	//ConsoleManager::GetInstance()->Print("alpha:%d\n", alpha_.Get());
	color_.SetA(alpha_);
	Renderer::GetInstance()->FillRect(0, 0, DeviceManager::GetInstance()->GetWidth(), DeviceManager::GetInstance()->GetHeight(), color_);
}
