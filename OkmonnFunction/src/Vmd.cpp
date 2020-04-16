#include "../include/Vmd.h"
#include <fstream>

namespace {
	struct VmdInfoHeader {
		char vertion[30];
		char model_name[20];
	};
	struct VmdMotion {
		char bone_name[15];
		std::uint32_t frame;
		okmonn::Vec3f pos;
		okmonn::Vec4f rotation;
		std::uint8_t interpolation[64];
	};
	struct VmdSkin {
		char skin_name[15];
		std::uint32_t frame;
		float weight;
	};
	struct VmdCamera {
		std::uint32_t frame;
		float distance;
		okmonn::Vec3f pos;
		okmonn::Vec3f rotation;
		std::uint8_t interpolation[24];
		std::uint32_t view_angle;
		std::uint8_t perspective;
	};
	struct VmdLight {
		std::uint32_t frame;
		okmonn::Vec3f color;
		okmonn::Vec3f pos;
	};
	struct VmdSelfShadow {
		std::uint32_t frame;
		std::uint8_t mode;
		float distance;
	};
}

std::uint32_t okmonn::LoadVmdFromFile(const std::string& file_path,
	std::unordered_map<std::string, std::vector<MotionData>>& motion_data)
{
	std::ifstream file(file_path.c_str(), std::ios::binary);
	if (!file.is_open()) {
		return 1;
	}

	VmdInfoHeader header{};
	file.read((char*)&header, sizeof(VmdInfoHeader));

	std::uint32_t motion_num = 0;
	file.read((char*)&motion_num, sizeof(std::uint32_t));
	std::vector<VmdMotion>motion(motion_num);
	for (auto& i : motion) {
		file.read((char*)&i.bone_name, sizeof(i.bone_name));
		file.read((char*)&i.frame, sizeof(i.frame));
		file.read((char*)&i.pos, sizeof(i.pos));
		file.read((char*)&i.rotation, sizeof(i.rotation));
		file.read((char*)&i.interpolation, sizeof(i.interpolation));

		motion_data[i.bone_name].push_back({i.frame, i.rotation});
	}

	std::uint32_t skin_num = 0;
	file.read((char*)&skin_num, sizeof(std::uint32_t));
	std::vector<VmdSkin>skin(skin_num);
	for (auto& i : skin) {
		file.read((char*)&i.skin_name, sizeof(i.skin_name));
		file.read((char*)&i.frame, sizeof(i.frame));
		file.read((char*)&i.weight, sizeof(i.weight));
	}

	std::uint32_t camera_num = 0;
	file.read((char*)&camera_num, sizeof(std::uint32_t));
	std::vector<VmdCamera>camera(camera_num);
	for (auto& i : camera) {
		file.read((char*)&i.frame, sizeof(i.frame));
		file.read((char*)&i.distance, sizeof(i.distance));
		file.read((char*)&i.pos, sizeof(i.pos));
		file.read((char*)&i.rotation, sizeof(i.rotation));
		file.read((char*)&i.interpolation, sizeof(i.interpolation));
		file.read((char*)&i.view_angle, sizeof(i.view_angle));
		file.read((char*)&i.perspective, sizeof(i.perspective));
	}

	std::uint32_t light_num = 0;
	file.read((char*)&light_num, sizeof(std::uint32_t));
	std::vector<VmdLight>light(light_num);
	for (auto i : light) {
		file.read((char*)&i, sizeof(VmdLight));
	}

	std::uint32_t shadow_num = 0;
	file.read((char*)&shadow_num, sizeof(std::uint32_t));
	std::vector<VmdSelfShadow>shadow(shadow_num);
	for (auto& i : shadow) {
		file.read((char*)&i.frame, sizeof(i.frame));
		file.read((char*)&i.mode, sizeof(i.mode));
		file.read((char*)&i.distance, sizeof(i.distance));
	}

	file.close();
	return 0;
}
