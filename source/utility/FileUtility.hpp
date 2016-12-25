#pragma once

#include <vector>
#include <string>
#include <algorithm>

class File
{
public:
	/*!
	 * @brief �o�C�i���t�@�C����ǂݍ���
	 *
	 * @param path �t�@�C���̃p�X
	 *
	 * @return �ǂݍ��񂾃o�C�i��
	 */
	static std::vector<char> ReadAllBytes(const std::string& path)
	{
		FILE* fp = std::fopen(path.c_str(), "rb");
		if (fp == nullptr) return std::vector<char>();
		std::fseek(fp, 0, SEEK_END);
		std::vector<char> buffer(ftell(fp));
		std::fseek(fp, 0, SEEK_SET);
		std::fread(&buffer[0], sizeof(char), buffer.size(), fp);
		std::fclose(fp);

		return buffer;
	}

	/*!
	 * @brief �e�L�X�g�t�@�C����ǂݍ���
	 *
	 * @param path �t�@�C���̃p�X
	 *
	 * @return �ǂݍ��񂾃e�L�X�g
	 */
	static std::string ReadAllText(const std::string& path)
	{
		FILE* fp = std::fopen(path.c_str(), "rb");
		if (fp == nullptr) return std::string();
		std::fseek(fp, 0, SEEK_END);
		std::string ret;
		size_t length = ftell(fp);
		ret.resize(ftell(fp));
		std::fseek(fp, 0, SEEK_SET);
		std::fread(&ret[0], sizeof(char), ret.length(), fp);
		std::fclose(fp);

		return ret;
	}

	/*!
	 * @brief �e�L�X�g�t�@�C�����s�ŕ������ēǂݍ���
	 *
	 * @param path �t�@�C���̃p�X
	 *
	 * @return �ǂݍ��񂾃e�L�X�g
	 */
	static std::vector<std::string> ReadAllLines(const std::string& path)
	{
		// �t�@�C���ǂݍ���
		auto str = File::ReadAllText(path);
		if (str.empty()) return std::vector<std::string>();

		// ���s�����擾
		size_t count = std::count(std::begin(str), std::end(str), '\n');
		if (str[str.length() - 1] != '\n') ++count;

		// �߂��ϐ���ݒ�
		std::vector<std::string> ret;
		ret.reserve(count);

		// ���ɂ���ĉ��s�R�[�h�ύX
		static const std::string token = "\r\n";

		// ��������
		const char* src = str.c_str();
		const char* end = str.c_str() + str.length();
		while (src < end) {
			const char* pp = std::strstr(src, token.c_str());
			if (pp == nullptr) {
				ret.push_back(std::string(src, end));
				break;
			}
			ret.push_back(std::string(src, pp - src));
			src = pp + token.length();
		}

		return ret;
	}
};
