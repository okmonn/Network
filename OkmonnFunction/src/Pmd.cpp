#include "../include/Pmd.h"
#include "../include/Function.h"
#include <fstream>

namespace {
	struct PmdInfoHeader {
		/*�o�[�W����*/
		float version;
		/*���f����*/
		char model_name[20];
		/*�R�����g*/
		char comment[256];
	};
	struct PmdVertexData {
		/*���_���W*/
		okmonn::Vec3f pos;
		/*�@���x�N�g��*/
		okmonn::Vec3f normal;
		/*UV���W*/
		okmonn::Vec2f uv;
		/*�e���{�[���ԍ�*/
		std::uint16_t bone_index[2];
		/*�e���x(bone_index[1] = 100 - bone_weight)*/
		std::uint8_t bone_weight;
		/*�֊s�t���O*/
		std::uint8_t edge_flag;
	};
	struct PmdMaterialData {
		/*�����F*/
		okmonn::Vec4f diffuse;
		/*���ʓx*/
		float supecularity;
		/*����F*/
		okmonn::Vec3f supecular;
		/*���F*/
		okmonn::Vec3f ambient;
		/*�g�D�[���e�N�X�`���ԍ�*/
		std::uint8_t toon_texture_index;
		/*�֊s�t���O*/
		std::uint8_t edge_flag;
		/*�C���f�b�N�X��*/
		std::uint32_t index_use_num;
		/*�e�N�X�`���t�@�C����*/
		char texture_file_name[20];
	};
	struct PmdBornData {
		/*�{�[����*/
		char bone_name[20];
		/*�e�{�[���ԍ�*/
		std::uint16_t parent_bone_index;
		/*�q�{�[���ԍ�*/
		std::uint16_t child_bone_index;
		/*�{�[���̎��*/
		std::uint8_t bone_type;
		/*IK�{�[���ԍ�(�����ꍇ=0)*/
		std::uint16_t ik_bone_index;
		/*�{�[�����W*/
		okmonn::Vec3f pos;
	};
	struct PmdIkBornData {
		/*IK�{�[���ԍ�*/
		std::uint16_t ik_bone_index;
		/*IK�^�[�Q�b�g�{�[���ԍ�*/
		std::uint16_t ik_target_index;
		/*�q�{�[����*/
		std::uint8_t ik_child_bone_num;
		/*�ċA���Z��*/
		std::uint16_t iterations;
		/*1�񂠂���̐����p�x*/
		float bone_limit_angle;
		/*�q�{�[���ԍ�*/
		std::vector<std::uint16_t>ik_child_bone_index;
	};
	struct PmdSkinVertex {
		/*�\��p�̃C���f�b�N�X*/
		std::uint32_t index;
		/*�\��p���W*/
		okmonn::Vec3f pos;
	};
	struct PmdSkinData {
		/*�\�*/
		char skin_name[20];
		/*�\��p�̒��_��*/
		std::uint32_t skin_vertex_num;
		/*�\��̎��*/
		std::uint8_t skin_type;
		/*�\��p���_�f�[�^*/
		std::vector<PmdSkinVertex>skin_vertex;
	};
	struct PmdBornFramData {
		/*�{�[���ԍ�*/
		std::uint16_t bone_index;
		/*�\���g�ԍ�*/
		std::uint8_t bone_fram_index;
	};
}

std::uint32_t okmonn::LoadPmdFromFile(const std::string& file_path, std::vector<okmonn::ModelVertex>& vertex_buffer, 
	std::vector<std::uint16_t>& index_buffer, std::vector<okmonn::ModelMaterial>& material_buffer, std::vector<std::uint32_t>& material_index,
	std::vector<std::string>& material_texture_path, std::unordered_map<std::string, ModelBorn>& bone_buffer)
{
	std::ifstream file(file_path.c_str(), std::ios::binary);
	if (!file.is_open()) {
		return 1;
	}

	/*�t�H�[�}�b�g��ʂ̊m�F*/
	unsigned char fmt_type[3];
	file.read((char*)fmt_type, sizeof(fmt_type));
	if (std::string(&fmt_type[0], &fmt_type[sizeof(fmt_type) / sizeof(fmt_type[0])]) != "Pmd") {
		file.close();
		return 1;
	}
	/*���w�b�_�����̓ǂݍ���*/
	PmdInfoHeader info_header{};
	file.read((char*)&info_header, sizeof(PmdInfoHeader));
	/*���_�f�[�^�����̓ǂݍ���*/
	std::uint32_t vertex_num = 0U;
	file.read((char*)&vertex_num, sizeof(std::uint32_t));
	vertex_buffer.resize(vertex_num);
	PmdVertexData vertex{};
	for (auto& i : vertex_buffer) {
		file.read((char*)&vertex, sizeof(vertex.pos) + sizeof(vertex.normal) + sizeof(vertex.uv) +
			sizeof(vertex.bone_index) + sizeof(vertex.bone_weight) + sizeof(vertex.edge_flag));
		
		okmonn::Copy((std::uint8_t*)i.bone_index, vertex.bone_index, sizeof(vertex.bone_index));
		i.bone_weight = vertex.bone_weight;
		i.normal      = vertex.normal;
		i.pos         = vertex.pos;
		i.uv          = vertex.uv;
	}
	/*�C���f�b�N�X�f�[�^�����̓ǂݍ���*/
	std::uint32_t index_num = 0U;
	file.read((char*)&index_num, sizeof(std::uint32_t));
	index_buffer.resize(index_num);
	file.read((char*)index_buffer.data(), sizeof(index_buffer[0]) * index_buffer.size());
	/*�}�e���A���f�[�^�����̓ǂݍ���*/
	std::uint32_t material_num = 0U;
	file.read((char*)&material_num, sizeof(std::uint32_t));
	material_buffer.resize(material_num);
	material_index.resize(material_num);
	material_texture_path.resize(material_num);
	PmdMaterialData material{};
	for(std::uint32_t i = 0; i < material_buffer.size(); ++i) {
		file.read((char*)&material.diffuse,            sizeof(material.diffuse));
		file.read((char*)&material.supecularity,       sizeof(material.supecularity));
		file.read((char*)&material.supecular,          sizeof(material.supecular));
		file.read((char*)&material.ambient,            sizeof(material.ambient));
		file.read((char*)&material.toon_texture_index, sizeof(material.toon_texture_index));
		file.read((char*)&material.edge_flag,          sizeof(material.edge_flag));
		file.read((char*)&material.index_use_num,      sizeof(material.index_use_num));
		file.read((char*)material.texture_file_name,   sizeof(material.texture_file_name));

		material_buffer[i].diffuse   = material.diffuse;
		material_buffer[i].supecular = okmonn::Vec4f(material.supecular, material.supecularity);
		material_buffer[i].ambient   = okmonn::Vec4f(material.ambient, 1.0f);

		material_index[i] = material.index_use_num;
		material_texture_path[i] = material.texture_file_name;
	}
	/*�{�[���f�[�^�����̓ǂݍ���*/
	std::uint16_t bone_num = 0U;
	file.read((char*)&bone_num, sizeof(std::uint16_t));
	std::vector<PmdBornData>bone(bone_num);
	for (auto& i : bone) {
		file.read(i.bone_name, sizeof(i.bone_name));
		file.read((char*)&i.parent_bone_index, sizeof(i.parent_bone_index));
		file.read((char*)&i.child_bone_index, sizeof(i.child_bone_index));
		file.read((char*)&i.bone_type, sizeof(i.bone_type));
		file.read((char*)&i.ik_bone_index, sizeof(i.ik_bone_index));
		file.read((char*)&i.pos, sizeof(i.pos));
	}
	for (std::uint32_t i = 0; i < bone.size(); ++i) {
		bone_buffer[bone[i].bone_name].bone_index = i;
		if (bone[i].parent_bone_index != 0xFFFF) {
			bone_buffer[bone[i].bone_name].head_pos = bone[i].pos;
			bone_buffer[bone[bone[i].parent_bone_index].bone_name].children.push_back(&bone_buffer[bone[i].bone_name]);
		}
		if (bone[i].child_bone_index != 0) {
			bone_buffer[bone[i].bone_name].tail_pos = bone[bone[i].child_bone_index].pos;
		}
	}

	/*IK�{�[���f�[�^�����̓ǂݍ���*/
	std::uint16_t ik_bone_num = 0U;
	file.read((char*)&ik_bone_num, sizeof(std::uint16_t));
	std::vector<PmdIkBornData>ik_bone(ik_bone_num);
	for (auto& i : ik_bone) {
		file.read((char*)&i.ik_bone_index, sizeof(i.ik_bone_index));
		file.read((char*)&i.ik_target_index, sizeof(i.ik_target_index));
		file.read((char*)&i.ik_child_bone_num, sizeof(i.ik_child_bone_num));
		file.read((char*)&i.iterations, sizeof(i.iterations));
		file.read((char*)&i.bone_limit_angle, sizeof(i.bone_limit_angle));
		i.ik_child_bone_index.resize(i.ik_child_bone_num);
		for (auto& n : i.ik_child_bone_index) {
			file.read((char*)&n, sizeof(n));
		}
	}
	/*�\��f�[�^�����̓ǂݍ���*/
	std::uint16_t skin_num = 0U;
	file.read((char*)&skin_num, sizeof(std::uint16_t));
	std::vector<PmdSkinData>skin(skin_num);
	for (auto& i : skin) {
		file.read((char*)&i.skin_name, sizeof(i.skin_name));
		file.read((char*)&i.skin_vertex_num, sizeof(i.skin_vertex_num));
		file.read((char*)&i.skin_type, sizeof(i.skin_type));
		i.skin_vertex.resize(i.skin_vertex_num);
		for (auto& n : i.skin_vertex) {
			file.read((char*)&n.index, sizeof(n.index));
			file.read((char*)&n.pos, sizeof(n.pos));
		}
	}
	/*�\��ԍ��f�[�^�����̓ǂݍ���*/
	std::uint8_t skin_index_num = 0U;
	file.read((char*)&skin_index_num, sizeof(std::uint8_t));
	std::vector<std::uint16_t>skin_index(skin_index_num);
	file.read((char*)skin_index.data(), sizeof(skin_index[0]) * skin_index.size());
	/*�{�[���g���f�[�^�����̓ǂݍ���*/
	std::uint8_t bone_fram_name_num = 0U;
	if (bone_fram_name_num != 0U) {
		std::vector<char[50]>bone_fram_name(bone_fram_name_num);
		file.read((char*)bone_fram_name.data(), sizeof(bone_fram_name[0]) * bone_fram_name.size());
	}
	/*�{�[���g�f�[�^�����̓ǂݍ���*/
	std::uint32_t bone_fram_num = 0U;
	if (bone_fram_num != 0U) {
		file.read((char*)&bone_fram_num, sizeof(std::uint32_t));
		std::vector<PmdBornFramData>bone_fram(bone_fram_num);
		for (auto& i : bone_fram) {
			file.read((char*)&i.bone_index, sizeof(i.bone_index));
			file.read((char*)&i.bone_fram_index, sizeof(i.bone_fram_index));
		}
	}

	file.close();
	return 0;
}
