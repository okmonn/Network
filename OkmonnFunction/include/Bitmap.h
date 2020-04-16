#ifndef OKMONN_BITMAP
#define OKMONN_BITMAP

#include "Vector.h"
#include <string>
#include <vector>

namespace okmonn {
	/*�r�b�g�}�b�v�t�@�C���̓ǂݍ���
	.�t�@�C���p�X
	.�摜�f�[�^�i�[�o�b�t�@
	.�����i�[�o�b�t�@
	.�c���i�[�o�b�t�@
	return �s�t���O(0�F�����@1�F���s)*/
	std::uint32_t LoadBitmapFromFile(const std::string& file_path,
		std::vector<std::uint8_t>& image_buffer, std::uint32_t* width = nullptr, std::uint32_t* height = nullptr);
}

#endif
