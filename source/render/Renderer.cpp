#include "Renderer.h"
#include "utility/Utility.hpp"
#include "main/WinMain.h"
#include <string.h>
#include <vector>
#include <string>

static const float RotateBase = 6.28318530718f;

D3DCMPFUNC ChangeFunc(Renderer::FUNC func)
{
	if (func == Renderer::NEVER) return D3DCMP_NEVER;
	if (func == Renderer::ALWAYS) return D3DCMP_ALWAYS;
	if (func == Renderer::LESS) return D3DCMP_LESS;
	if (func == Renderer::EQUAL) return D3DCMP_EQUAL;
	if (func == Renderer::LESSEQUAL) return D3DCMP_LESSEQUAL;
	if (func == Renderer::GREATER) return D3DCMP_GREATER;
	if (func == Renderer::NOTEQUAL) return D3DCMP_NOTEQUAL;
	if (func == Renderer::GREATEREQUAL) return D3DCMP_GREATEREQUAL;
	return D3DCMP_FORCE_DWORD;
}

uint32_t ChangeStencilCaps(Renderer::STENCIL_CAPS caps)
{
	if (caps == Renderer::STENCIL_CAPS_KEEP) return D3DSTENCILCAPS_KEEP;
	if (caps == Renderer::STENCIL_CAPS_ZERO) return D3DSTENCILCAPS_ZERO;
	if (caps == Renderer::STENCIL_CAPS_REPLACE) return D3DSTENCILCAPS_REPLACE;
	if (caps == Renderer::STENCIL_CAPS_INVERT) return D3DSTENCILCAPS_INVERT;

	return 0;
}

Renderer::Renderer()
	: app_(nullptr)
	, device_lost_(false)
{
}

Renderer::~Renderer()
{
}

/*!
 * @brief ������
 */
void Renderer::Initialize(AppBase* app)
{
	Finalize();
	app_ = app;
	auto device = GetDevice_();
	if (device == nullptr) return ;

	app_->GetDevice().AddDeviceLostListener(this);
	stocks_.resize(1024);
	stock_count_ = 0;
}

/*!
 * @brief �I����
 */
void Renderer::Finalize()
{
	for (auto it = fonts_.begin(); it != fonts_.end(); ++it) {
		it->second->Release();
	}
	fonts_.clear();
	std::vector<Vertex2D>().swap(stocks_);
}

void Renderer::SetTextureFilter(bool filter)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (filter) {
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);		// �~�b�v�}�b�v
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �g��
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �k��
	} else {
		device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);		// �~�b�v�}�b�v
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);		// �g��
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);		// �k��
	}
}

void Renderer::SetAlphaTest(bool enable, FUNC func, uint8_t ref)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (enable) {
		D3DCMPFUNC f = ChangeFunc(func);
		if (f == D3DCMP_FORCE_DWORD) return ;

		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, f);
		device->SetRenderState(D3DRS_ALPHAREF, ref);
	} else {
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

void Renderer::SetBlend(BLEND blend)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (blend == BLEND_DISABLE) {
		// ����
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	} else if (blend == BLEND_ALPHA) {
		// �A���t�@�u�����h
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	} else if (blend == BLEND_ADD) {
		// ���Z
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	} else if (blend == BLEND_SUB) {
		// ���Z
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	} else if (blend == BLEND_MUL) {
		// ��Z
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	}
}

void Renderer::SetTextureUse(TEXTURE_USE tex)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (tex == TEXTURE_USE_DISABLE) {
		// ����
	} else if (tex == TEXTURE_USE_ENABLE) {
		// �L��
	} else if (tex == TEXTURE_USE_COLOR) {
		// �J���[�̂ݗL��
	} else if (tex == TEXTURE_USE_ALPHA) {
		// �A���t�@�̂ݗL��
	}
}

void Renderer::SetStencilState(STENCIL_STATAE stencil)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (stencil == STENCIL_DISABLE) {
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	} else if (stencil == STENCIL_MASK) {
		// �X�e���V���ݒ�
		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		device->SetRenderState(D3DRS_STENCILFUNC,   D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_STENCILREF,    0x01);
		device->SetRenderState(D3DRS_STENCILPASS,   D3DSTENCILOP_REPLACE);
		device->SetRenderState(D3DRS_STENCILZFAIL,  D3DSTENCILOP_REPLACE);
		device->SetRenderState(D3DRS_STENCILMASK,   0xff);
	}else if (stencil == STENCIL_UNMASK) {
		// �X�e���V���ݒ�
		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		device->SetRenderState(D3DRS_STENCILFUNC,   D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_STENCILREF,    0x00);
		device->SetRenderState(D3DRS_STENCILPASS,   D3DSTENCILOP_REPLACE);
		device->SetRenderState(D3DRS_STENCILZFAIL,  D3DSTENCILOP_REPLACE);
		device->SetRenderState(D3DRS_STENCILMASK,   0xff);
	}else if (stencil == STENCIL_DRAW) {
		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		device->SetRenderState(D3DRS_STENCILFUNC,   D3DCMP_EQUAL);
		device->SetRenderState(D3DRS_STENCILREF,    0x00);
		device->SetRenderState(D3DRS_STENCILPASS,   D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILZFAIL,  D3DSTENCILOP_KEEP);
		device->SetRenderState(D3DRS_STENCILMASK,   0xff);
	}
}

void Renderer::SetStencil(bool enable, FUNC func, uint8_t ref, STENCIL_CAPS sfail, STENCIL_CAPS zfail, STENCIL_CAPS zpass)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (enable) {
		D3DCMPFUNC f = ChangeFunc(func);
		uint32_t s1 = ChangeStencilCaps(sfail);
		uint32_t z1 = ChangeStencilCaps(zfail);
		uint32_t z2 = ChangeStencilCaps(zpass);
		if (f == D3DCMP_FORCE_DWORD || s1 == 0 || z1 == 0 || z2 == 0) return ;

		device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		device->SetRenderState(D3DRS_STENCILFUNC,   f);
		device->SetRenderState(D3DRS_STENCILMASK,   0xff);
		device->SetRenderState(D3DRS_STENCILFAIL,   s1);
		device->SetRenderState(D3DRS_STENCILPASS,   z1);
		device->SetRenderState(D3DRS_STENCILZFAIL,  z2);
	} else {
		device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
}

void Renderer::SetZEnable(bool enable, FUNC func)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	if (enable) {
		D3DCMPFUNC f = ChangeFunc(func);
		if (f == D3DCMP_FORCE_DWORD) return ;

		device->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
		device->SetRenderState(D3DRS_ZFUNC, f);
	} else {
		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}
}

void Renderer::SetZWriteEnable(bool enable)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	device->SetRenderState(D3DRS_ZWRITEENABLE, enable ? TRUE : FALSE);
}

/*!
 * @brief ��ʃN���A
 */
void Renderer::ClearScreen(const Color& color)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	device->Clear(0, nullptr, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, color.GetARGB(), 1.0f, 0);
}

/*!
 * @brief �h��Ԃ��`��
 */
void Renderer::FillRect(int x, int y, int w, int h, const Color& color)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	const float left = static_cast<float>(x);
	const float top = static_cast<float>(y);
	const float right = static_cast<float>(w) + left;
	const float bottom = static_cast<float>(h) + top;
	const uint32_t col = color.GetARGB();

	struct VERTEX2D {
		float x, y, z, rhw;
		uint32_t color;
	} v[] = {
		left,  top,    0.0f, 1.0f, col,
		right, top,    0.0f, 1.0f, col,
		left,  bottom, 0.0f, 1.0f, col,
		right, bottom, 0.0f, 1.0f, col,
	};

	HRESULT hr;
	hr = device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	hr = device->SetTexture(0, nullptr);
	hr = device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX2D));
}

/*!
 * @brief �摜�`��
 */
void Renderer::DrawImage(Texture* texture, Anchor anchor, int x, int y)
{
	if (texture == nullptr) return ;
	DrawImage(texture, anchor, x, y, texture->GetWidth(), texture->GetHeight(), 0, 0, texture->GetWidth(), texture->GetHeight());
}

/*!
 * @brief �摜�`��
 */
void Renderer::DrawImage(Texture* texture, Anchor anchor, int dx, int dy, int dw, int dh,
	int sx, int sy, int sw, int sh, const Color& color, float rotate)
{
	auto device = GetDevice_();
	if (device == nullptr) return ;
	if (texture == nullptr) return ;
	if (color.GetA() == 0) return ;

	Vertex2D v[4];
	CreateVertex2D_(v, texture, anchor, dx, dy, dw, dh, sx, sy, sw, sh, color, rotate);

	// �`��
	HRESULT hr;
	hr = device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	texture->Apply();
	hr = device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
}


/*!
 * @brief �X�g�b�N�N���A
 */
void Renderer::ClearStock()
{
	stock_count_ = 0;
}

/*!
 * @brief �摜�X�g�b�N
 */
void Renderer::ImageStock(Texture* texture, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Color& color, float rotate)
{
	if (texture == nullptr) return ;
	if (color.GetA() == 0) return ;

	size_t index = stock_count_ * 4;
	if (index >= stocks_.size()) {
		stocks_.resize(stocks_.size() * 2);
	}
	CreateVertex2D_(&stocks_[index], texture, anchor, dx, dy, dw, dh, sx, sy, sw, sh, color, rotate);
	++stock_count_;
}

/*!
 * @brief �X�g�b�N�`��
 */
void Renderer::DrawStock()
{
	auto device = GetDevice_();
	if (device == nullptr) return ;

	/*
	// �`��
	HRESULT hr;
	hr = device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	hr = device->SetTexture(0, texture->GetTexture());
	hr = device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(Vertex2D));
	*/
}

/*!
 * @brief ������`��
 */
void Renderer::DrawString(const char* s, Anchor anchor, int x, int y, int size, const Color& color)
{
	if (color.GetA() == 0) return ;

	this->DrawString(Utility::SJIStoUTF16(s).c_str(), anchor, x, y, size, color);
}

/*!
 * @brief ������`��
 */
void Renderer::DrawString(const wchar_t* s, Anchor anchor, int x, int y, int size, const Color& color)
{
	if (color.GetA() == 0) return ;

	auto* font = GetFont_(size);
	if (font == nullptr) return ;

	RECT rect;
	rect.left = x;
	rect.right = x + 1;
	rect.top = y;
	rect.bottom = y + 1;

	// �A���J�[����ɂ��ĕ\�����W��ς���
	if (anchor != LEFT_TOP) {
		int height = font->DrawTextW(nullptr, s, -1, &rect, DT_CALCRECT | DT_SINGLELINE, color.GetARGB());
		int width = rect.right - rect.left;
		rect.left -= width  / 2 * (anchor % 3);
		rect.top -= height / 2 * (anchor / 3);
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;
	}

	font->DrawTextW(nullptr, s, wcslen(s), &rect, DT_NOCLIP, color.GetARGB());
}

/*!
 * @brief �����`��
 */
void Renderer::DrawStringFormat(Anchor anchor, int x, int y, int size, const Color& color, const char* s, ...)
{
	if (color.GetA() == 0) return ;

	// �e�L�X�g�t�H�[�}�b�g��ϊ�
	char text[10240];
	va_list arg;
	va_start(arg, s);
	vsprintf(text, s, arg);
	va_end(arg);

	this->DrawString(Utility::SJIStoUTF16(text).c_str(), anchor, x, y, size, color);
}

/*!
 * @brief �����`��
 */
void Renderer::DrawStringFormat(Anchor anchor, int x, int y, int size, const Color& color, const wchar_t* s, ...)
{
	if (color.GetA() == 0) return ;

	// �e�L�X�g�t�H�[�}�b�g��ϊ�
	wchar_t text[10240];
	va_list arg;
	va_start(arg, s);
	vswprintf(text, s, arg);
	va_end(arg);

	this->DrawString(text, anchor, x, y, size, color);
}

void Renderer::OnLostDevice()
{
	for (auto it = fonts_.begin(); it != fonts_.end(); ++it) {
		it->second->OnLostDevice();
	}
	device_lost_ = true;
}

void Renderer::OnResetDevice()
{
	for (auto it = fonts_.begin(); it != fonts_.end(); ++it) {
		it->second->OnResetDevice();
	}
	device_lost_ = false;
}

/*!
 * @brief �t�H���g�̎擾
 */
LPD3DXFONT Renderer::GetFont_(int size)
{
	if (device_lost_) return nullptr;

	auto it = fonts_.find(size);
	if (it != fonts_.end()) {
		return fonts_[size];
	}

	auto device = app_->GetDevice().GetDevice();
	if (device == nullptr) return nullptr;

	LPD3DXFONT font;
	D3DXCreateFont(device, size, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"�l�r �S�V�b�N", &font);
	fonts_.insert(std::pair<int, LPD3DXFONT>(size, font));
	return font;
}

void Renderer::CreateVertex2D_(Vertex2D* v, Texture* texture, Anchor anchor, int dx, int dy, int dw, int dh, int sx, int sy, int sw, int sh, const Color& color, float rotate)
{
	if (texture == nullptr) return ;
	if (color.GetA() == 0) return ;

	dx -= (dw >> 1) * (anchor % 3);
	dy -= (dh >> 1) * (anchor / 3);

	const float left = static_cast<float>(dx);
	const float top = static_cast<float>(dy);
	const float right = static_cast<float>(dw) + left;
	const float bottom = static_cast<float>(dh) + top;
	const float tl = static_cast<float>(sx + 0.5f) / texture->GetWidth();
	const float tt = static_cast<float>(sy + 0.5f) / texture->GetHeight();
	const float tr = static_cast<float>(sw) / texture->GetWidth() + tl;
	const float tb = static_cast<float>(sh) / texture->GetHeight() + tt;
	const uint32_t col = color.GetARGB();

	Vertex2D* v0 = &v[0];
	Vertex2D* v1 = &v[1];
	Vertex2D* v2 = &v[2];
	Vertex2D* v3 = &v[3];
	v0->x = left;  v0->y = top;    v0->z = 0.0f; v0->rhw = 1.0f; v0->color = col; v0->u = tl; v0->v = tt;
	v1->x = right; v1->y = top;    v1->z = 0.0f; v1->rhw = 1.0f; v1->color = col; v1->u = tr; v1->v = tt;
	v2->x = left;  v2->y = bottom; v2->z = 0.0f; v2->rhw = 1.0f; v2->color = col; v2->u = tl; v2->v = tb;
	v3->x = right; v3->y = bottom; v3->z = 0.0f; v3->rhw = 1.0f; v3->color = col; v3->u = tr; v3->v = tb;

	// ��]�s�����������
	if (rotate != 0) {
		D3DXMATRIX mat, matt;
		D3DXMatrixTranslation(&mat, -left, -top, 0);
		mat *= *D3DXMatrixRotationZ(&matt, rotate * RotateBase);
		mat.m[3][0] += left;
		mat.m[3][1] += top;

		for (int i = 0; i < 4; ++i) {
			D3DXVECTOR3 din, dout;
			din.x = v[i].x;
			din.y = v[i].y;
			din.z = v[i].z;
			D3DXVec3TransformCoord(&dout, &din, &mat);
			v[i].x = dout.x;
			v[i].y = dout.y;
			v[i].z = dout.z;
		}
	}
}

LPDIRECT3DDEVICE9 Renderer::GetDevice_()
{
	if (app_ == nullptr) return nullptr;
	return app_->GetDevice().GetDevice();
}
