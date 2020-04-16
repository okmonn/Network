#ifndef OKMONN_BITMAP
#define OKMONN_BITMAP

#include "Vector.h"
#include <string>
#include <vector>

namespace okmonn {
	/*ビットマップファイルの読み込み
	.ファイルパス
	.画像データ格納バッファ
	.横幅格納バッファ
	.縦幅格納バッファ
	return 不可フラグ(0：成功　1：失敗)*/
	std::uint32_t LoadBitmapFromFile(const std::string& file_path,
		std::vector<std::uint8_t>& image_buffer, std::uint32_t* width = nullptr, std::uint32_t* height = nullptr);
}

#endif
