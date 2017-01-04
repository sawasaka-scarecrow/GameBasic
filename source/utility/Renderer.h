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
	void Initialize(HDC hdc);

	/*!
	 * @brief ��ʃN���A
	 */
	void ClearScreen(const Gdiplus::Color& color);

	/*!
	 * @brief �h��Ԃ��`��
	 */
	void FillRect(int x, int y, int w, int h, const Gdiplus::Color& color);

	/*!
	 * @brief �摜�`��
	 */
	void DrawImage(Gdiplus::Bitmap* image, Anchor anchor, int x, int y);

	/*!
	 * @brief �摜�`��
	 */
	void DrawImage(Gdiplus::Bitmap* image, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Gdiplus::Color& color);

	/*!
	 * @brief ������`��
	 */
	void DrawString(const char* s, Anchor anchor, int x, int y, int size = 20, const Gdiplus::Color& color = Gdiplus::Color::White);

	/*!
	 * @brief ������`��
	 */
	void DrawString(const wchar_t* s, Anchor anchor, int x, int y, int size = 20, const Gdiplus::Color& color = Gdiplus::Color::White);

	/*!
	 * @brief �t�H�[�}�b�g�w�蕶����`��
	 */
	void DrawStringFormat(Anchor anchor, int x, int y, int size, const Gdiplus::Color& color, const char* s, ...);

	/*!
	 * @brief �t�H�[�}�b�g�w�蕶����`��
	 */
	void DrawStringFormat(Anchor anchor, int x, int y, int size, const Gdiplus::Color& color, const wchar_t* s, ...);

	/*!
	 * @brief �C���X�^���X�擾
	 */
	static Renderer* GetInstance() {
		static Renderer v;
		return &v;
	}

private :
	Renderer();
	Gdiplus::Font* GetFont(int size);

	Gdiplus::GdiplusStartupInput gpsi_;
	ULONG_PTR token_;
	std::shared_ptr<Gdiplus::Graphics> graphics_;
	std::map<int, std::shared_ptr<Gdiplus::Font>> fonts_;
	std::shared_ptr<Gdiplus::SolidBrush> brush_;
	Gdiplus::ImageAttributes ia_;
	Gdiplus::ColorMatrix cm_;
	Gdiplus::StringFormat string_format_;
};
