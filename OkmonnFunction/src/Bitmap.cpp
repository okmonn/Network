#include "../include/Bitmap.h"
#include <fstream>

namespace {
	struct BitmapFileHeader {
		/*�t�@�C���T�C�Y*/
		std::uint32_t size;
		/*�\��̈�*/
		std::uint16_t reserved1;
		/*�\��̈�*/
		std::uint16_t reserved2;
		/*�t�@�C���擪����摜�f�[�^�܂ł̃I�t�Z�b�g*/
		std::uint32_t offset;
	};
	struct BitmapCoreHeader {
		/*�w�b�_�T�C�Y*/
		std::uint32_t size;
		/*����*/
		std::uint16_t width;
		/*�c��*/
		std::uint16_t height;
		/*�v���[����*/
		std::uint16_t plane;
		/*�s�N�Z���P�ʂ̃r�b�g��*/
		std::uint16_t depth;
	};
	struct BitmapInfoNormalHeader {
		/*�w�b�_�T�C�Y*/
		std::uint32_t size;
		/*����*/
		std::uint32_t width;
		/*�c��*/
		std::uint32_t height;
		/*�v���[����*/
		std::uint16_t plane;
		/*�s�N�Z���P�ʂ̃r�b�g��*/
		std::uint16_t depth;
		/*���k�^�C�v*/
		std::uint32_t compression;
		/*�摜�T�C�Y*/
		std::uint32_t image_size;
		/*�����𑜓x*/
		std::uint32_t horizontal_resolution;
		/*�����𑜓x*/
		std::uint32_t vertical_resolution;
		/*�J���[�C���f�b�N�X��*/
		std::uint32_t color_index;
		/*�d�v�C���f�b�N�X*/
		std::uint32_t important_index;
	};
	struct BitmapInfoV4Header {
		/*�J���[�}�X�N*/
		std::uint32_t color_mask[4];
		/*�F��ԃ^�C�v*/
		std::uint32_t color_space;
		/*�F��ԍ��W*/
		okmonn::Vector3<std::uint32_t>color_pos[3];
		/*�K���}�l*/
		std::uint32_t color_gamma[3];
	};
	struct BitmapInfoV5Header {
		/*sRGB�F��ԃ^�C�v*/
		std::uint32_t sRGB;
		/*�F��ԃv���t�B�[���I�t�Z�b�g*/
		std::uint32_t profile_offset;
		/*�F��ԃv���t�B�[���T�C�Y*/
		std::uint32_t profile_size;
		/*�\��̈�*/
		std::uint32_t reserved;
	};
	struct BitmapInfoHeader {
		BitmapInfoNormalHeader normal;
		BitmapInfoV4Header v4;
		BitmapInfoV5Header v5;
	};
}

std::uint32_t okmonn::LoadBitmapFromFile(const std::string& file_path, 
	std::vector<std::uint8_t>& image_buffer, std::uint32_t* width, std::uint32_t* height)
{
	std::ifstream file(file_path.c_str(), std::ios::binary);
	if (!file.is_open()) {
		return 1;
	}
	/*�t�H�[�}�b�g��ʂ̊m�F*/
	unsigned char fmt_type[2];
	file.read((char*)fmt_type, sizeof(fmt_type));
	if (std::string(&fmt_type[0], &fmt_type[sizeof(fmt_type) / sizeof(fmt_type[0])]) != "BM") {
		file.close();
		return 1;
	}

	/*�t�@�C���w�b�_�����̓ǂݍ���*/
	BitmapFileHeader file_header{};
	file.read((char*)&file_header, sizeof(BitmapFileHeader));
	/*���w�b�_�����̓ǂݍ���*/
	BitmapInfoHeader info_header{};
	file.read((char*)&info_header.normal, sizeof(BitmapInfoNormalHeader));
	if (width != nullptr) {
		(*width) = info_header.normal.width;
	}
	if (height != nullptr) {
		(*height) = info_header.normal.height;
	}
	switch (info_header.normal.size - sizeof(BitmapInfoNormalHeader)) {
		case sizeof(BitmapInfoV4Header):
			file.read((char*)&info_header.v4, sizeof(BitmapInfoV4Header));
			break;
		case sizeof(BitmapInfoV4Header) + sizeof(BitmapInfoV5Header):
			file.read((char*)&info_header.v4, sizeof(BitmapInfoV4Header));
			file.read((char*)&info_header.v5, sizeof(BitmapInfoV5Header));
			break;
		default:
			break;
	}
	/*�摜�f�[�^�����̓ǂݍ���*/
	file.seekg(file_header.offset, std::ios::beg);
	image_buffer.resize(sizeof(std::uint32_t) * info_header.normal.width * info_header.normal.height);
	image_buffer.shrink_to_fit();
	switch (info_header.normal.depth) {
	case 24:
	{
		std::vector<std::uint8_t>color(info_header.normal.depth / 8);
		for (std::int32_t y = info_header.normal.height - 1; y >= 0; --y) {
			for (std::uint32_t x = 0U; x < info_header.normal.width; ++x) {
				file.read((char*)color.data(), color.size());
				std::reverse(color.begin(), color.end());
				std::uint32_t index = y * info_header.normal.width * 4 + x * 4;
				for (std::uint32_t i = 0U; i < color.size(); ++i) {
					image_buffer[index + i] = color[i];
				}
				image_buffer[index + color.size()] = 0xFFU;
			}
		}
		break;
	}
	case 32:
	{
		std::uint32_t color = 0U;
		for (std::int32_t y = info_header.normal.height - 1; y >= 0; --y) {
			for (std::uint32_t x = 0U; x < info_header.normal.width; ++x) {
				file.read((char*)&color, sizeof(std::uint32_t));
				std::uint32_t index = y * info_header.normal.width * 4 + x * 4;
				for (std::uint32_t i = 0U; i < 4; ++i) {
					std::uint32_t tmp = color & info_header.v4.color_mask[i];
					if ((tmp & 0xFFFF0000U) != 0U) {
						tmp >>= 16;
					}
					if ((tmp & 0xFF00U) != 0U) {
						tmp >>= 8;
					}
					image_buffer[index + i] = std::uint8_t(tmp);
				}
				if (image_buffer[index + 3] == 0U) {
					image_buffer[index + 3] = 0xFFU;
				}
			}
		}
		break;
	}
	default:
		break;
	}

	file.close();
	return 0U;
}
