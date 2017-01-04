#include "FadeManager.h"
#include "Renderer.h"
#include "DeviceManager.h"
#include "ConsoleManager.h"

/*!
 * @brief �t�F�[�h�C��
 */
void FadeManager::FadeIn(float time, const Gdiplus::Color& color)
{
	alpha_.Set(255, 0, time, Bezier::Linear);
	ConsoleManager::GetInstance()->Print("alpfa:Set\n");
}

/*!
 * @brief �t�F�[�h�A�E�g
 */
void FadeManager::FadeOut(float time, const Gdiplus::Color& color)
{
	alpha_.Set(0, 255, time, Bezier::Linear);
}

/*!
 * @brief �X�V
 */
void FadeManager::Update(float df)
{
	alpha_.Update(df);
}

/*!
 * @brief �`��
 */
void FadeManager::Render()
{
	if (alpha_ == 0) return ;
	//ConsoleManager::GetInstance()->Print("alpfa:%d\n", alpha_.Get());

	color_.SetValue((color_.GetValue() & ~Gdiplus::Color::AlphaMask) | ((alpha_ & 0xff) << Gdiplus::Color::AlphaShift));
	Renderer::GetInstance()->FillRect(0, 0, DeviceManager::GetInstance()->GetWidth(), DeviceManager::GetInstance()->GetHeight(), color_);
}
