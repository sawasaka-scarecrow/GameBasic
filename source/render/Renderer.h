/*!
 * @file
 *
 * @brief �����_�����O�N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include <Windows.h>
#include <memory>
#include <d3d9.h>
#include "master/MasterData.hpp"
#include "main/DeviceLostListener.h"
#include "Texture.h"
#include "Color.h"

class AppBase;

/*!
 * @brief �����_�����O�N���X
 */
class Renderer : public DeviceLostListener
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

	enum BLEND
	{
		BLEND_DISABLE,			//!< ����
		BLEND_ALPHA,			//!< �A���t�@�u�����h
		BLEND_ADD,				//!< ���Z
		BLEND_SUB,				//!< ���Z
		BLEND_MUL,				//!< ��Z
		BLEND_DEST,				//!< Z��X�e���V���̂ݏ�������
	};

	enum TEXTURE_USE
	{
		TEXTURE_USE_DISABLE,	//!< ����
		TEXTURE_USE_ENABLE,		//!< �L��
		TEXTURE_USE_COLOR,		//!< �J���[�̂ݗL��
		TEXTURE_USE_ALPHA,		//!< �A���t�@�̂ݗL��
	};

	enum STENCIL_STATAE
	{
		STENCIL_DISABLE,		//!< ����
		//!< �ȉ��X�e���V���͗L���ݒ�ł��B
		STENCIL_MASK,			//!< �}�X�N�̍쐬
		STENCIL_UNMASK,			//!< �}�X�N�̍쐬
		STENCIL_DRAW,			//!< �K�p�ς̃}�X�N�ɏ]���`��͈̓N���b�s���O���܂�
	};

	enum STENCIL_CAPS
	{
		STENCIL_CAPS_KEEP,		//!< �X�e���V���o�b�t�@�̍��ڂ��X�V���Ȃ�
		STENCIL_CAPS_ZERO,		//!< �X�e���V���o�b�t�@�̍��ڂ�0�ɐݒ肷��
		STENCIL_CAPS_REPLACE,	//!< �X�e���V���o�b�t�@�̍��ڂ���l�Œu��������
		STENCIL_CAPS_INVERT,	//!< �X�e���V���o�b�t�@�̍��ڂ𔽓]����
	};

	enum FUNC
	{
		NEVER,					//!< �e�X�g�͏�Ɏ��s����B
		ALWAYS,					//!< �e�X�g�͏�ɐ�������B
		EQUAL,					//!< �V�����s�N�Z���l���A���݂̃s�N�Z���l�Ɠ������Ƃ��ɉ�����B
		NOTEQUAL,				//!< �V�����s�N�Z���l���A���݂̃s�N�Z���l�Ɠ������Ȃ��Ƃ��ɉ�����B
		LESS,					//!< �V�����s�N�Z���l���A���݂̃s�N�Z���l��菬�����Ƃ��ɉ�����B 
		LESSEQUAL,				//!< �V�����s�N�Z���l���A���݂̃s�N�Z���l�ȉ��̂Ƃ��ɉ�����B
		GREATER,				//!< �V�����s�N�Z���l���A���݂̃s�N�Z���l���傫���Ƃ��ɉ�����B
		GREATEREQUAL,			//!< �V�����s�N�Z���l���A���݂̃s�N�Z���l�ȏ�̂Ƃ��ɉ�����B
	};

	struct Vertex2D {
		float x, y, z, rhw;
		uint32_t color;
		float u, v;
	};

	~Renderer();

	/*!
	 * @brief ������
	 */
	void Initialize(AppBase* app);

	/*!
	 * @brief �I����
	 */
	void Finalize();

	/*!
	 * @brief �e�N�X�`���[�t�B���^�̗L����ݒ肵�܂��B
	 *
	 * @param filter �e�N�X�`���[�t�B���^�̗L�����w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetTextureFilter(bool filter);

	/*!
	 * @brief �A���t�@�e�X�g�̗L����ݒ肵�܂��B
	 *
	 * @param enable �A���t�@�e�X�g�̗L�����w�肵�܂��B
	 * @param func ����֐����w�肵�܂��B
	 * @param ref �e�X�g�̒l���w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetAlphaTest(bool enable, FUNC func = NOTEQUAL, uint8_t ref = 0);

	/*!
	 * @brief �u�����h���[�h��ύX���܂��B
	 *
	 * @param blend �u�����h���[�h���w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetBlend(BLEND blend);

	/*!
	 * @brief �e�N�X�`���[�̎g�p��Ԃ�ύX���܂��B
	 *
	 * @param tex �e�N�X�`���[�̎g�p��Ԃ��w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetTextureUse(TEXTURE_USE tex);

	/*!
	 * @brief �X�e���V���X�e�[�^�X��ݒ肵�܂�
	 *
	 * @param stencil �X�e���V���X�e�[�^�X���e���w�肵�܂�
	 *
	 * @return �Ȃ�
	 */
	void SetStencilState(STENCIL_STATAE stencil);

	/*!
	 * @brief �X�e���V���X�e�[�^�X��ݒ肵�܂�
	 *
	 * @param enable �X�e���V���̗L�����w�肵�܂��B
	 * @param func ����֐����w�肵�܂��B
	 * @param ref �e�X�g�̒l���w�肵�܂��B
	 * @param sfail �X�e���V���e�X�g���s���̃I�y���[�V�������w�肵�܂��B
	 * @param zfail �X�e���V���e�X�g�������f�v�X�e�X�g���s���̃I�y���[�V�������w�肵�܂��B
	 * @param zpass �X�e���V���e�X�g�������f�v�X�e�X�g�������̃I�y���[�V�������w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetStencil(bool enable, FUNC func = NOTEQUAL, uint8_t ref = 0, STENCIL_CAPS sfail = STENCIL_CAPS_KEEP, STENCIL_CAPS zfail = STENCIL_CAPS_KEEP, STENCIL_CAPS zpass = STENCIL_CAPS_KEEP);

	/*!
	 * @brief Z�o�b�t�@�̗L����ݒ肵�܂��B
	 *
	 * @param enable �X�e���V���X�e�[�^�X���e���w�肵�܂�
	 *
	 * @return �Ȃ�
	 */
	void SetZEnable(bool enable, FUNC func = LESSEQUAL);

	/*!
	 * @brief Z�o�b�t�@�̏������݂̗L����ݒ肵�܂��B
	 *
	 * @param enable Z�o�b�t�@�̏������݂̗L�����w�肵�܂�
	 *
	 * @return �Ȃ�
	 */
	void SetZWriteEnable(bool enable);

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
	 * @brief �X�g�b�N�N���A
	 */
	void ClearStock();

	/*!
	 * @brief �摜�X�g�b�N
	 */
	void ImageStock(Texture* texture, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Color& color = Color::White, float rotate = 0.0f);

	/*!
	 * @brief �X�g�b�N�`��
	 */
	void DrawStock();

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
	virtual void OnLostDevice();
	virtual void OnResetDevice();

private :
	Renderer();
	LPD3DXFONT GetFont_(int size);
	void CreateVertex2D_(Vertex2D* v, Texture* texture, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Color& color, float rotate);
	LPDIRECT3DDEVICE9 GetDevice_();

	AppBase* app_;
	std::map<int, LPD3DXFONT> fonts_;
	std::vector<Vertex2D> stocks_;
	size_t stock_count_;
	bool device_lost_;
};
