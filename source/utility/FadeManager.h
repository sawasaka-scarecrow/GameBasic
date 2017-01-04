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
	void FadeIn(float time = 0.3f, const Gdiplus::Color& color = Gdiplus::Color::Black);

	/*!
	 * @brief �t�F�[�h�A�E�g
	 */
	void FadeOut(float time = 0.3f, const Gdiplus::Color& color = Gdiplus::Color::Black);

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
	FadeManager(){}

	Bezier::Timer<int> alpha_;
	Gdiplus::Color color_;
};
