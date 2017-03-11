/*!
* @file
*
* @brief �L�[�Ǘ��N���X��񋟂��܂��B
*
* @copyright
*/
#pragma once

class KeyCode
{
public :
	enum Code {
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		N0,
		N1,
		N2,
		N3,
		N4,
		N5,
		N6,
		N7,
		N8,
		N9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		CLEAR,
		ENTER,
		SHIFT,
		CONTROL,
		MENU,
		PAUSE,
		CAPITAL,
		KANA,
		HANGEUL,
		HANGUL,
		JUNJA,
		FINAL,
		HANJA,
		KANJI,
		ESCAPE,
		CONVERT,
		NONCONVERT,
		ACCEPT,
		MODECHANGE,
		SPACE,
		PRIOR,
		NEXT,
		END,
		HOME,
		LEFT,
		UP,
		RIGHT,
		DOWN,
		SELECT,
		PRINT,
		EXECUTE,
		SNAPSHOT,
		INSERT,
		DEL,
		HELP,
	};
};

/*!
* @brief �L�[�Ǘ��N���X
*/
class Key
{
public :
	/*!
	* @brief �L�[�̃N���A
	*/
	void Clear();

	/*!
	* @brief ��������
	*/
	void Down(unsigned char key);

	/*!
	* @brief ��������
	*/
	void Up(unsigned char key);

	/*!
	* @brief �L�[�̍X�V
	*/
	void Update();

	/*!
	* @brief �L�[�̉�������ςȂ����擾
	*/
	bool IsPress(KeyCode::Code code) const;

	/*!
	* @brief �L�[�̉����������擾
	*/
	bool IsTrg(KeyCode::Code code) const;

	/*!
	* @brief �L�[�̗����������擾
	*/
	bool IsRelease(KeyCode::Code code) const;

	/*!
	* @brief �L�[���s�[�g
	*/
	bool IsRepeat(KeyCode::Code code) const;

	static Key* GetInstance() {
		static Key v;
		return &v;
	}

private :
	Key();

	struct Info {
		bool key;
		bool key_back;
		bool key_temp;
		int key_count;
	};
	Info infos_[256];
};
