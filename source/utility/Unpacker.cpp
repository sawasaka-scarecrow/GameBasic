#include "Unpacker.hpp"
#include "stream/FileStream.hpp"
#include "stream/MemoryStream.hpp"
#include "stream/BinaryReader.hpp"
#include <algorithm>
#include "Utility.hpp"

void Unpacker::Set(const char* path)
{
	auto stream = FileStream(path, FileStream::TypeRead);
	auto reader = BinaryReader(stream);

	// �t�@�C����
	int value = reader.ReadInt32();

	// �w�b�_�[�o�b�t�@�擾
	auto header_size = reader.ReadUInt32();
	std::vector<char> buf = reader.ReadBytes(header_size);
	auto header_stream = MemoryStream(buf);
	auto header_reader = BinaryReader(header_stream);

	// �t�@�C�����擾
	infos_.resize(value);
	for (int i = 0; i < value; ++i) {
		auto& info = infos_[i];
		info.offset = header_reader.ReadInt64();
		info.size = header_reader.ReadInt64();
		int length = header_reader.ReadInt32();
		auto b = header_reader.ReadBytes(length);
		auto name = std::string(&b[0], length);
		auto crc = Utility::GetCrc32(name.c_str(), name.length());
		maps_[crc] = &info;
	}
	header_reader.Close();

	// �t�@�C���f�[�^���擾
	for (int i = 0; i < value; ++i) {
		auto& info = infos_[i];
		info.buf = reader.ReadBytes(static_cast<int>(info.size));
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
	auto crc = Utility::GetCrc32(name.c_str(), name.length());
	auto it = maps_.find(crc);
	if (it == maps_.end()) {
		return empty_;
	}
	return it->second->buf;
}
