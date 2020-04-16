#include "../include/Bitmap.h"
#include <fstream>

namespace {
	struct BitmapFileHeader {
		/*ファイルサイズ*/
		std::uint32_t size;
		/*予約領域*/
		std::uint16_t reserved1;
		/*予約領域*/
		std::uint16_t reserved2;
		/*ファイル先頭から画像データまでのオフセット*/
		std::uint32_t offset;
	};
	struct BitmapCoreHeader {
		/*ヘッダサイズ*/
		std::uint32_t size;
		/*横幅*/
		std::uint16_t width;
		/*縦幅*/
		std::uint16_t height;
		/*プレーン数*/
		std::uint16_t plane;
		/*ピクセル単位のビット数*/
		std::uint16_t depth;
	};
	struct BitmapInfoNormalHeader {
		/*ヘッダサイズ*/
		std::uint32_t size;
		/*横幅*/
		std::uint32_t width;
		/*縦幅*/
		std::uint32_t height;
		/*プレーン数*/
		std::uint16_t plane;
		/*ピクセル単位のビット数*/
		std::uint16_t depth;
		/*圧縮タイプ*/
		std::uint32_t compression;
		/*画像サイズ*/
		std::uint32_t image_size;
		/*水平解像度*/
		std::uint32_t horizontal_resolution;
		/*垂直解像度*/
		std::uint32_t vertical_resolution;
		/*カラーインデックス数*/
		std::uint32_t color_index;
		/*重要インデックス*/
		std::uint32_t important_index;
	};
	struct BitmapInfoV4Header {
		/*カラーマスク*/
		std::uint32_t color_mask[4];
		/*色空間タイプ*/
		std::uint32_t color_space;
		/*色空間座標*/
		okmonn::Vector3<std::uint32_t>color_pos[3];
		/*ガンマ値*/
		std::uint32_t color_gamma[3];
	};
	struct BitmapInfoV5Header {
		/*sRGB色空間タイプ*/
		std::uint32_t sRGB;
		/*色空間プロフィールオフセット*/
		std::uint32_t profile_offset;
		/*色空間プロフィールサイズ*/
		std::uint32_t profile_size;
		/*予約領域*/
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
	/*フォーマット種別の確認*/
	unsigned char fmt_type[2];
	file.read((char*)fmt_type, sizeof(fmt_type));
	if (std::string(&fmt_type[0], &fmt_type[sizeof(fmt_type) / sizeof(fmt_type[0])]) != "BM") {
		file.close();
		return 1;
	}

	/*ファイルヘッダ部分の読み込み*/
	BitmapFileHeader file_header{};
	file.read((char*)&file_header, sizeof(BitmapFileHeader));
	/*情報ヘッダ部分の読み込み*/
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
	/*画像データ部分の読み込み*/
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
