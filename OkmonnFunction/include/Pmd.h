#ifndef OKMONN_PMD
#define OKMONN_PMD

#include "Vector.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace okmonn {
	struct ModelVertex {
		/*座標*/
		okmonn::Vec3f pos;
		/*UV座標*/
		okmonn::Vec2f uv;
		/*法線ベクトル*/
		okmonn::Vec3f normal;
		/*ボーン番号*/
		std::uint16_t bone_index[2];
		/*ウェイト*/
		std::uint8_t bone_weight;
	};
	struct ModelMaterial {
		/*減衰色*/
		okmonn::Vec4f diffuse;
		/*鏡面色*/
		okmonn::Vec4f supecular;
		/*環境色*/
		okmonn::Vec4f ambient;
	};
	struct ModelBorn {
		/*ボーン番号*/
		std::uint32_t bone_index;
		/*先頭座標*/
		okmonn::Vec3f head_pos;
		/*終端座標*/
		okmonn::Vec3f tail_pos;
		/*子ボーンのポインタ*/
		std::vector<ModelBorn*>children;
	}; 

	/*PMDファイルの読み込み
	.ファイルパス
	.頂点情報格納バッファ
	.インデックス情報格納バッファ
	.マテリアル情報格納バッファ
	.マテリアルごとのインデックス数
	.マテリアルごとのテクスチャパス
	.ボーン情報格納バッファ
	return 不可フラグ(0：成功　1：失敗)*/
	std::uint32_t LoadPmdFromFile(const std::string& file_path, std::vector<ModelVertex>& vertex_buffer,
		std::vector<std::uint16_t>& index_buffer, std::vector<ModelMaterial>& material_buffer, std::vector<std::uint32_t>& material_index,
		std::vector<std::string>& material_texture_path, std::unordered_map<std::string, ModelBorn>& bone_buffer);
}

#endif
