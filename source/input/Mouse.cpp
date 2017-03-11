#include "Mouse.h"
#include <string.h>
#include <algorithm>
#include "master/MasterData.hpp"

Mouse::Mouse()
	: hwnd_(NULL)
{
	Clear();
}

/*!
 * @brief �N���A
 */
void Mouse::Clear()
{
	memset(infos_, false , sizeof(infos_));
	pos_x_ = 0;
	pos_y_ = 0;
	pos_x_back_ = 0;
	pos_y_back_ = 0;
	z_delta_ = 0;
	wheel_fraction_ = 0;
	notch_ = 0;
	ReleaseCapture();
}

/*!
 * @brief �C�x���g
 */
void Mouse::WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	hwnd_ = hWnd;
	if (msg == WM_LBUTTONDOWN) {
		infos_[LBUTTON].button_temp = true;
		SetCapture(hWnd);
	} else if (msg == WM_LBUTTONUP) {
		infos_[LBUTTON].button_temp = false;
		ReleaseCapture();
	} else if (msg == WM_MBUTTONDOWN) {
		infos_[MBUTTON].button_temp = true;
		SetCapture(hWnd);
	} else if (msg == WM_MBUTTONUP) {
		infos_[MBUTTON].button_temp = false;
		ReleaseCapture();
	} else if (msg == WM_RBUTTONDOWN) {
		infos_[RBUTTON].button_temp = true;
		SetCapture(hWnd);
	} else if (msg == WM_RBUTTONUP) {
		infos_[RBUTTON].button_temp = false;
		ReleaseCapture();
	} else if (msg == WM_MOUSEWHEEL) {
		z_delta_ = GET_WHEEL_DELTA_WPARAM(wParam) + wheel_fraction_;
		wheel_fraction_ = z_delta_ % WHEEL_DELTA;
	}
}

/*!
 * @brief �{�^���̍X�V
 */
void Mouse::Update()
{
	for (int i = 0; i < sizeof(infos_) / sizeof(*infos_); ++i) {
		auto& info = infos_[i];
		info.button_back = info.button;
		info.button = info.button_temp;
	}
	notch_ = z_delta_ / WHEEL_DELTA;
	z_delta_ = 0;

	POINT pos;
	if (GetCursorPos(&pos)) {
		if (ScreenToClient(hwnd_, &pos)) {
			pos_x_back_ = pos_x_;
			pos_y_back_ = pos_y_;
			pos_x_ = pos.x;
			pos_y_ = pos.y;
		}
	}
}

/*!
 * @brief �{�^���̉�������ςȂ����擾
 */
bool Mouse::IsPress(Button button) const
{
	return infos_[button].button;
}

/*!
 * @brief �{�^���̃h���b�O���擾
 */
bool Mouse::IsDrag(Button button) const
{
	return IsPress(button) && !IsTrg(button);
}

/*!
 * @brief �{�^���̉����������擾
 */
bool Mouse::IsTrg(Button button) const
{
	return infos_[button].button && !infos_[button].button_back;
}

/*!
 * @brief �{�^���̗����������擾
 */
bool Mouse::IsRelease(Button button) const
{
	return !infos_[button].button && infos_[button].button_back;
}

/*!
 * @brief �z�C�[������ɉ񂳂ꂽ�Ƃ����擾
 */
bool Mouse::IsHwheelUp() const
{
	return notch_ > 0;
}

/*!
 * @brief �z�C�[�������ɉ񂳂ꂽ�Ƃ����擾
 */
bool Mouse::IsHwheelDown() const
{
	return notch_ < 0;
}

/*!
 * @brief �}�E�X��X���W���擾
 */
int Mouse::GetX() const
{
	return pos_x_;
}

/*!
 * @brief �}�E�X��Y���W���擾
 */
int Mouse::GetY() const
{
	return pos_y_;
}

/*!
 * @brief �}�E�X��X�̈ړ��ʂ��擾
 */
int Mouse::GetMoveX() const
{
	return pos_x_ - pos_x_back_;
}

/*!
 * @brief �}�E�X��Y�̈ړ��ʂ��擾
 */
int Mouse::GetMoveY() const
{
	return pos_y_ - pos_y_back_;
}
