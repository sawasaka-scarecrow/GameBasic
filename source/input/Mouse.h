/*!
 * @file
 *
 * @brief �}�E�X�Ǘ��N���X��񋟂��܂��B
 *
 * @copyright
 */
#pragma once

#include <Windows.h>

/*!
 * @brief �}�E�X�Ǘ��N���X
 */
class Mouse
{
public :
	enum Button {
		LBUTTON,		//!< ���N���b�N
		MBUTTON,		//!< �����N���b�N
		RBUTTON,		//!< �E�N���b�N

		MAX,
	};

	/*!
	 * @brief �}�E�X�̃N���A
	 */
	void Clear();

	/*!
	 * @brief �C�x���g
	 */
	void WndProc(HWND hWnd, UINT msg, UINT wParam, LONG lParam);

	/*!
	 * @brief �X�V
	 */
	void Update();

	/*!
	 * @brief �{�^���̉�������ςȂ����擾
	 */
	bool IsPress(Button button) const;

	/*!
	 * @brief �{�^���̃h���b�O���擾
	 */
	bool IsDrag(Button button) const;

	/*!
	 * @brief �{�^���̉����������擾
	 */
	bool IsTrg(Button button) const;

	/*!
	 * @brief �{�^���̗����������擾
	 */
	bool IsRelease(Button button) const;

	/*!
	 * @brief �z�C�[������ɉ񂳂ꂽ�Ƃ����擾
	 */
	bool IsHwheelUp() const;

	/*!
	 * @brief �z�C�[�������ɉ񂳂ꂽ�Ƃ����擾
	 */
	bool IsHwheelDown() const;

	/*!
	 * @brief �}�E�X��X���W���擾
	 */
	int GetX() const;

	/*!
	 * @brief �}�E�X��Y���W���擾
	 */
	int GetY() const;

	/*!
	 * @brief �}�E�X��X�̈ړ��ʂ��擾
	 */
	int GetMoveX() const;

	/*!
	 * @brief �}�E�X��Y�̈ړ��ʂ��擾
	 */
	int GetMoveY() const;

	static Mouse* GetInstance() {
		static Mouse v;
		return &v;
	}

private :
	Mouse();

	struct Info {
		bool button;
		bool button_back;
		bool button_temp;
	};
	Info infos_[MAX];

	int pos_x_;
	int pos_y_;
	int pos_x_back_;
	int pos_y_back_;
	int z_delta_;
	int wheel_fraction_;
	int notch_;
	HWND hwnd_;
};
