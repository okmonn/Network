#ifndef OKMONN_PMD
#define OKMONN_PMD

#include "Vector.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace okmonn {
	struct ModelVertex {
		/*���W*/
		okmonn::Vec3f pos;
		/*UV���W*/
		okmonn::Vec2f uv;
		/*�@���x�N�g��*/
		okmonn::Vec3f normal;
		/*�{�[���ԍ�*/
		std::uint16_t bone_index[2];
		/*�E�F�C�g*/
		std::uint8_t bone_weight;
	};
	struct ModelMaterial {
		/*�����F*/
		okmonn::Vec4f diffuse;
		/*���ʐF*/
		okmonn::Vec4f supecular;
		/*���F*/
		okmonn::Vec4f ambient;
	};
	struct ModelBorn {
		/*�{�[���ԍ�*/
		std::uint32_t bone_index;
		/*�擪���W*/
		okmonn::Vec3f head_pos;
		/*�I�[���W*/
		okmonn::Vec3f tail_pos;
		/*�q�{�[���̃|�C���^*/
		std::vector<ModelBorn*>children;
	}; 

	/*PMD�t�@�C���̓ǂݍ���
	.�t�@�C���p�X
	.���_���i�[�o�b�t�@
	.�C���f�b�N�X���i�[�o�b�t�@
	.�}�e���A�����i�[�o�b�t�@
	.�}�e���A�����Ƃ̃C���f�b�N�X��
	.�}�e���A�����Ƃ̃e�N�X�`���p�X
	.�{�[�����i�[�o�b�t�@
	return �s�t���O(0�F�����@1�F���s)*/
	std::uint32_t LoadPmdFromFile(const std::string& file_path, std::vector<ModelVertex>& vertex_buffer,
		std::vector<std::uint16_t>& index_buffer, std::vector<ModelMaterial>& material_buffer, std::vector<std::uint32_t>& material_index,
		std::vector<std::string>& material_texture_path, std::unordered_map<std::string, ModelBorn>& bone_buffer);
}

#endif
