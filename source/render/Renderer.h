/*!
 * @file
 *
 * @brief �����_�����O�N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#include "master/MasterData.hpp"
#include "Texture.h"
#include "Color.h"

class AppBase;

/*!
 * @brief �����_�����O�N���X
 */
class Renderer
{
public :
	enum Anchor
	{
		LEFT_TOP,
		TOP,
		RIGHT_TOP,
		LEFT,
		CENTER,
		RIGHT,
		LEFT_BOTTOM,
		BOTTOM,
		RIGHT_BOTTOM,
	};

	~Renderer();

	/*!
	 * @brief ������
	 */
	void Initialize(AppBase* app);

	/*!
	 * @brief ��ʃN���A
	 */
	void ClearScreen(const Color& color);

	/*!
	 * @brief �h��Ԃ��`��
	 */
	void FillRect(int x, int y, int w, int h, const Color& color);

	/*!
	 * @brief �摜�`��
	 */
	void DrawImage(Texture* texture, Anchor anchor, int x, int y);

	/*!
	 * @brief �摜�`��
	 */
	void DrawImage(Texture* texture, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Color& color = Color::White, float rotate = 0.0f);

	/*!
	 * @brief ������`��
	 */
	void DrawString(const char* s, Anchor anchor, int x, int y, int size = 20, const Color& color = Color::White);

	/*!
	 * @brief ������`��
	 */
	void DrawString(const wchar_t* s, Anchor anchor, int x, int y, int size = 20, const Color& color = Color::White);

	/*!
	 * @brief �t�H�[�}�b�g�w�蕶����`��
	 */
	void DrawStringFormat(Anchor anchor, int x, int y, int size, const Color& color, const char* s, ...);

	/*!
	 * @brief �t�H�[�}�b�g�w�蕶����`��
	 */
	void DrawStringFormat(Anchor anchor, int x, int y, int size, const Color& color, const wchar_t* s, ...);

	/*!
	 * @brief �C���X�^���X�擾
	 */
	static Renderer* GetInstance() {
		static Renderer v;
		return &v;
	}

private :
	Renderer();

	AppBase* app_;
};
