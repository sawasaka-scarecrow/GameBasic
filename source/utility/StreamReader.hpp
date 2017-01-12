#pragma once

#include <vector>
#include <string>

class StreamReader
{
public:
	StreamReader() : buffer_(nullptr), offset_(0)
	{
	}

	StreamReader(std::vector<char>& buffer) : buffer_(&buffer), offset_(0)
	{
	}

	/*!
	 * @brief bool�^�̓ǂݍ���
	 *
	 * @return �ǂݍ���bool�^�̒l
	 */
	bool ReadBool()
	{
		if (offset_ + sizeof(bool) > buffer_->size()) return 0;
		auto ret = (*(unsigned char*)&(*buffer_)[offset_]) != 0;
		offset_ += sizeof(bool);
		return ret;
	}

	/*!
	 * @brief bool�^�̓ǂݍ���(�V�[�N�Ȃ�)
	 *
	 * @return �ǂݍ���bool�^�̒l
	 */
	bool ReadBoolNoSeek()
	{
		auto temp = offset_;
		auto ret = ReadBool();
		offset_ = temp;
		return ret;
	}

	/*!
	 * @brief int�^�̓ǂݍ���
	 *
	 * @return �ǂݍ���int�^�̒l
	 */
	int ReadInt()
	{
		if (offset_ + sizeof(int) > buffer_->size()) return 0;
		auto ret = *(int*)&(*buffer_)[offset_];
		offset_ += sizeof(int);
		return ret;
	}

	/*!
	 * @brief int�^�̓ǂݍ���(�V�[�N�Ȃ�)
	 *
	 * @return �ǂݍ���int�^�̒l
	 */
	int ReadIntNoSeek()
	{
		auto temp = offset_;
		auto ret = ReadInt();
		offset_ = temp;
		return ret;
	}

	/*!
	 * @brief float�^�̓ǂݍ���
	 *
	 * @return �ǂݍ���float�^�̒l
	 */
	float ReadFloat()
	{
		if (offset_ + sizeof(float) > buffer_->size()) return 0;
		auto ret = *(float*)&(*buffer_)[offset_];
		offset_ += sizeof(float);
		return ret;
	}

	/*!
	 * @brief float�^�̓ǂݍ���(�V�[�N�Ȃ�)
	 *
	 * @return �ǂݍ���float�^�̒l
	 */
	float ReadFloatNoSeek()
	{
		auto temp = offset_;
		auto ret = ReadFloat();
		offset_ = temp;
		return ret;
	}

	/*!
	 * @brief ������̓ǂݍ���
	 *
	 * @return �ǂݍ��񂾕�����
	 */
	std::string ReadString()
	{
		int len = ReadInt();
		std::string ret = std::string(&(*buffer_)[offset_], len);
		offset_ += ret.length();
		return ret;
	}

	/*!
	 * @brief ������̓ǂݍ���(�V�[�N�Ȃ�)
	 *
	 * @return �ǂݍ��񂾕�����
	 */
	std::string ReadStringNoSeek()
	{
		auto temp = offset_;
		auto ret = ReadString();
		offset_ = temp;
		return ret;
	}

private :
	std::vector<char>* buffer_;
	size_t offset_;
};
