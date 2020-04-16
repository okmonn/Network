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

	/*VMDファイルの読み込み
	return 不可フラグ(0：成功　1：失敗)*/
	std::uint32_t LoadVmdFromFile(const std::string& file_path, 
		std::unordered_map<std::string, std::vector<MotionData>>& motion_data);
}

#endif