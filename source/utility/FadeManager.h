/*!
 * @file
 *
 * @brief �t�F�[�h�Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include "bezier.h"
#include <Windows.h>
#include <gdiplus.h>

/*!
 * @brief �t�F�[�h�Ǘ��N���X��񋟂��܂��B
 */
class FadeManager
{
public :
	/*!
	 * @brief �t�F�[�h�C��
	 */
	void FadeIn(float time = 0.3f, const Gdiplus::Color& color = Gdiplus::Color::Black, float delay = 0.0f);

	/*!
	 * @brief �t�F�[�h�A�E�g
	 */
	void FadeOut(float time = 0.3f, const Gdiplus::Color& color = Gdiplus::Color::Black, float delay = 0.0f);

	/*!
	 * @brief �I������
	 */
	bool IsEnd() const { return alpha_.IsEnd(); }

	/*!
	 * @brief �X�V
	 */
	void Update(float df);

	/*!
	 * @brief �`��
	 */
	void Render();

	static FadeManager* GetInstance() {
		static FadeManager v;
		return &v;
	}

private :
	FadeManager() : delay_time_(0.0f), color_(Gdiplus::Color::Black) { alpha_.Set(255, 255, 0.0f, Bezier::Linear); }

	float delay_time_;
	Gdiplus::Color color_;
	Bezier::Timer<int> alpha_;
};
