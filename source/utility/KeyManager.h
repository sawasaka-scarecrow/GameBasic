#pragma once

class KeyManager
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
	bool IsPress(unsigned char key) const;

	/*!
	 * @brief �L�[�̉����������擾
	 */
	bool IsTrg(unsigned char key) const;

	/*!
	 * @brief �L�[�̗����������擾
	 */
	bool IsRelease(unsigned char key) const;

	static KeyManager* GetInstance() {
		static KeyManager v;
		return &v;
	}

private :
	KeyManager();
	bool keys_[256];
	bool keys_back_[256];
	bool keys_temp_[256];
};
