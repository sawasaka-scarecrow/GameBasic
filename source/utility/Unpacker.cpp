#include "Unpacker.hpp"
#include "stream/FileStream.hpp"
#include "stream/BinaryReader.hpp"
#include <algorithm>
#include "Utility.hpp"

void Unpacker::Set(const char* path)
{
	auto stream = FileStream(path, true);
	auto reader = BinaryReader(stream);

	// �t�@�C����
	int value = reader.ReadInt32();
	// �w�b�_�[�T�C�Y
	reader.ReadUInt32();
	// �t�@�C�����擾
	infos_.resize(value);
	for (int i = 0; i < value; ++i) {
		auto& info = infos_[i];
		info.offset = reader.ReadInt64();
		info.size = reader.ReadInt64();
		int length = reader.ReadInt32();
		auto b = reader.ReadBytes(length);
		auto name = std::string(&b[0], length);
		auto crc = Utility::GetCrc32(
			name.c_str(), name.length());
		maps_[crc] = &info;
	}
	// �t�@�C���f�[�^���擾
	for (int i = 0; i < value; ++i) {
		auto& info = infos_[i];
		info.buf = reader.ReadBytes(
			static_cast<int>(info.size));
	}

	reader.Close();
}

const std::vector<char>& Unpacker::GetData(int index)
{
	if (0 <= index && index < static_cast<int>(infos_.size())) {
		return infos_[index].buf;
	}
	return empty_;
}

const std::vector<char>& Unpacker::GetData(const std::string& name)
{
	auto crc = Utility::GetCrc32(
		name.c_str(), name.length());
	auto it = maps_.find(crc);
	if (it == maps_.end()) {
		return empty_;
	}
	return it->second->buf;
}
