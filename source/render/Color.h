/*!
 * @file
 * 
 * @brief �J���[�N���X��񋟂��܂�
 * 
 * @copyright
 */
#pragma once

#include <cstdint>

/*!
 * @brief RGB888�`���̃s�N�Z���t�H�[�}�b�g��\���N���X�ł��B
 */
class Color{
public :
	static Color Black;
	static Color White;

public :
	/*!
	 * @brief �R���X�g���N�^
	 *
	 *        0 �ŏ���������܂��B
	 *
	 * @param �Ȃ�
	 */
	Color(){ Clear(); }

	/*!
	 * @brief �R���X�g���N�^
	 *
	 * @param argb ARGB8888�`���̒l���w�肵�܂��B
	 */
	Color(uint32_t argb){ SetARGB(argb); }

	/*!
	 * @brief �R���X�g���N�^
	 * 
	 * @param r �Ԃ̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param g �΂̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param b �̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 */
	Color(uint8_t r, uint8_t g, uint8_t b){ SetARGB(255, r, g, b); }

	/*!
	 * @brief �R���X�g���N�^
	 * 
	 * @param a �A���t�@�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param r �Ԃ̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param g �΂̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param b �̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 */
	Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b){ SetARGB(a, r, g, b); }

	/*!
	 * @brief �Ԃ��擾���܂��B
	 *
	 * @param �Ȃ�
	 *
	 * @return �Ԃ̐F��Ԃ��܂��B
	 */
	uint8_t GetR() const{ return r_; }

	/*!
	 * @brief �Ԃ��Z�b�g���܂��B
	 *
	 * @param r �Ԃ̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetR(uint8_t r){ r_ = r; }

	/*!
	 * @brief �΂��擾���܂��B
	 *
	 * @param �Ȃ�
	 *
	 * @return �΂̐F��Ԃ��܂��B
	 */
	uint8_t GetG() const{ return g_; }

	/*!
	 * @brief �΂��Z�b�g���܂��B
	 *
	 * @param g �΂̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetG(uint8_t g){ g_ = g; }

	/*!
	 * @brief ���擾���܂��B
	 *
	 * @param �Ȃ�
	 *
	 * @return �̐F��Ԃ��܂��B
	 */
	uint8_t GetB() const{ return b_; }

	/*!
	 * @brief ���Z�b�g���܂��B
	 *
	 * @param b �̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetB(uint8_t b){ b_ = b; }

	/*!
	 * @brief �A���t�@���擾���܂��B
	 *
	 * @param �Ȃ�
	 *
	 * @return �A���t�@�̒l��Ԃ��܂��B
	 */
	uint8_t GetA() const{ return a_; }

	/*!
	 * @brief �A���t�@���Z�b�g���܂��B
	 *
	 * @param a �A���t�@�̒l�� @b 0�`255 �Ŏw�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetA(uint8_t a) { a_ = a; }

	/*!
	 * @brief �s�N�Z�����Z�b�g���܂��B
	 * 
	 * @param r �Ԃ̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param g �΂̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param b �̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetRGB(uint8_t r, uint8_t g, uint8_t b){ r_ = r; g_ = g; b_ = b; }

	/*!
	 * @brief �s�N�Z�����Z�b�g���܂��B
	 * 
	 * @param rgb RGB888�`���̒l���w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetRGB(uint32_t rgb){ SetRGB((uint8_t)(rgb >> 0), (uint8_t)(rgb >> 8), (uint8_t)(rgb >> 16)); }

	/*!
	 * @brief �s�N�Z�����擾���܂��B
	 * 
	 * @param �Ȃ�
	 *
	 * @return �s�N�Z���̒l��RGB888�`���ŕԂ��܂��B
	 */
	uint32_t GetRGB() const{ return (b_ << 16) | (g_ << 8) | (r_ << 0); }

	/*!
	 * @brief �s�N�Z�����Z�b�g���܂��B
	 * 
	 * @param a �A���t�@�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param r �Ԃ̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param g �΂̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 * @param b �̐F�� @b 0�`255 �Ŏw�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b){ a_ = a; r_ = r; g_ = g; b_ = b; }

	/*!
	 * @brief �s�N�Z�����Z�b�g���܂��B
	 * 
	 * @param argb ARGB8888�`���̒l���w�肵�܂��B
	 *
	 * @return �Ȃ�
	 */
	void SetARGB(uint32_t argb){ SetARGB((uint8_t)(argb >> 24), (uint8_t)(argb >> 0), (uint8_t)(argb >> 8), (uint8_t)(argb >> 16)); }
	
	/*!
	 * @brief �s�N�Z�����擾���܂��B
	 * 
	 * @param �Ȃ�
	 *
	 * @return �s�N�Z���̒l��ARGB8888�`���ŕԂ��܂��B
	 */
	uint32_t GetARGB() const{ return (a_ << 24) | (r_ << 16) | (g_ << 8) | (b_ << 0); }

	/*!
	 * @brief �s�N�Z�����擾���܂��B
	 * 
	 * @param �Ȃ�
	 *
	 * @return �s�N�Z���̒l��RGBA8888�`���ŕԂ��܂��B
	 */
	uint32_t GetRGBA() const { return (r_ << 24) | (a_ << 16) | (b_ << 24) | (g_ << 0); }

	/*!
	 * @brief �l���N���A���܂��B
	 *
	 * @param �Ȃ�
	 *
	 * @return �Ȃ�
	 */
	void Clear(){ a_ = 0; r_ = 0; g_ = 0; b_ = 0; }

	/*!
	 * @brief ���g��operator�ł��B
	 */
	Color& operator=(const Color& src){
		r_ = src.r_;
		g_ = src.g_;
		b_ = src.b_;
		a_ = src.a_;
		return (*this);
	}

private:
	uint8_t b_;			//!< ���̃s�N�Z����B�l�ł��B
	uint8_t g_;			//!< ���̃s�N�Z����G�l�ł��B
	uint8_t r_;			//!< ���̃s�N�Z����R�l�ł��B
	uint8_t a_;			//!< ���̃s�N�Z����A�l�ł��B
};
