#ifndef OKMONN_FUNCTION
#define OKMONN_FUNCTION

#include "Matrix.h"

namespace okmonn {
	/*ビュー行列の取得
	.カメラ座標
	.ターゲット座標
	.アップベクトル
	return ビュー行列*/
	okmonn::Matrix4x4 GetViewMatrix(const okmonn::Vec3f& pos, const okmonn::Vec3f& target,
		const okmonn::Vec3f& up = okmonn::Vec3f(0.0f, 1.0f, 0.0f));
	/*パースペクティブ行列の取得
	.視野角ラジアン角度
	.アスペクト比
	.近値
	.遠値
	return パースペクティブ行列*/
	okmonn::Matrix4x4 GetPerspectiveMatrix(const float& fov, const float& aspect, const float& near_z, const float& far_z);
}

namespace okmonn {
	/*メモリコピー
	.コピー先バッファ
	.コピー元バッファ
	.コピーサイズ*/
	template <typename T>
	void Copy(T* dst, const void* src, const std::uint32_t& copy_size) {
		for (std::uint32_t i = 0; i < copy_size / sizeof(T); ++i) {
			dst[i] = ((T*)src)[i];
		}
	}
	/*弧度法に変換
	.ラジアン角度
	return 弧度法*/
	template <typename T>
	T ConvertArcDegree(const T& radian) {
		return radian * T(180.0) / std::acos(T(-1.0));
	}
	/*ラジアン角度に変換
	.弧度法
	return ラジアン角度*/
	template <typename T>
	T ConvertRadian(const T& angle) {
		return angle * std::acos(T(-1.0)) / T(180.0);
	}
}

#endif
