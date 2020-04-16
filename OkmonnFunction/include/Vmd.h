#ifndef OKMONN_VMD
#define OKMONN_VMD

#include "Vector.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace okmonn {
	struct MotionData {
		std::uint32_t frame;
		okmonn::Vec4f rotation;
	};

	/*VMD�t�@�C���̓ǂݍ���
	return �s�t���O(0�F�����@1�F���s)*/
	std::uint32_t LoadVmdFromFile(const std::string& file_path, 
		std::unordered_map<std::string, std::vector<MotionData>>& motion_data);
}

#endif