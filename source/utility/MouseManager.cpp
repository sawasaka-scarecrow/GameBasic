#include "MouseManager.h"
#include <string.h>
#include <algorithm>
#include "master/MasterData.hpp"

MouseManager::MouseManager()
	: hwnd_(NULL)
{
	Clear();
}

/*!
 * @brief �N���A
 */
void MouseManager::Clear()
{
	memset(infos_, false , sizeof(infos_));
	pos_x_ = 0;
	pos_y_ = 0;
	pos_x_back_ = 0;
	pos_y_back_ = 0;
	z_delta_ = 0;
	wheel_fraction_ = 0;
	notch_ = 0;
}

/*!
 * @brief �C�x���g
 */
void MouseManager::WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam)
{
	hwnd_ = hWnd;
	if (msg == WM_LBUTTONDOWN) {
		infos_[LBUTTON].button_temp = true;
	} else if (msg == WM_LBUTTONUP) {
		infos_[LBUTTON].button_temp = false;
	} else if (msg == WM_MBUTTONDOWN) {
		infos_[MBUTTON].button_temp = true;
	} else if (msg == WM_MBUTTONUP) {
		infos_[MBUTTON].button_temp = false;
	} else if (msg == WM_RBUTTONDOWN) {
		infos_[RBUTTON].button_temp = true;
	} else if (msg == WM_RBUTTONUP) {
		infos_[RBUTTON].button_temp = false;
	} else if (msg == WM_MOUSEWHEEL) {
		z_delta_ = GET_WHEEL_DELTA_WPARAM(wParam) + wheel_fraction_;
		wheel_fraction_ = z_delta_ % WHEEL_DELTA;
	}
}

/*!
 * @brief �{�^���̍X�V
 */
void MouseManager::Update()
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
bool MouseManager::IsPress(Button button) const
{
	return infos_[button].button;
}

/*!
 * @brief �{�^���̃h���b�O���擾
 */
bool MouseManager::IsDrag(Button button) const
{
	return IsPress(button) && !IsTrg(button);
}

/*!
 * @brief �{�^���̉����������擾
 */
bool MouseManager::IsTrg(Button button) const
{
	return infos_[button].button && !infos_[button].button_back;
}

/*!
 * @brief �{�^���̗����������擾
 */
bool MouseManager::IsRelease(Button button) const
{
	return !infos_[button].button && infos_[button].button_back;
}

/*!
 * @brief �z�C�[������ɉ񂳂ꂽ�Ƃ����擾
 */
bool MouseManager::IsHwheelUp() const
{
	return notch_ > 0;
}

/*!
 * @brief �z�C�[�������ɉ񂳂ꂽ�Ƃ����擾
 */
bool MouseManager::IsHwheelDown() const
{
	return notch_ < 0;
}

/*!
 * @brief �}�E�X��X���W���擾
 */
int MouseManager::GetX() const
{
	return pos_x_;
}

/*!
 * @brief �}�E�X��Y���W���擾
 */
int MouseManager::GetY() const
{
	return pos_y_;
}

/*!
 * @brief �}�E�X��X�̈ړ��ʂ��擾
 */
int MouseManager::GetMoveX() const
{
	return pos_x_ - pos_x_back_;
}

/*!
 * @brief �}�E�X��Y�̈ړ��ʂ��擾
 */
int MouseManager::GetMoveY() const
{
	return pos_y_ - pos_y_back_;
}
