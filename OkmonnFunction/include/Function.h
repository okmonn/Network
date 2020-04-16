#ifndef OKMONN_FUNCTION
#define OKMONN_FUNCTION

#include "Matrix.h"

namespace okmonn {
	/*�r���[�s��̎擾
	.�J�������W
	.�^�[�Q�b�g���W
	.�A�b�v�x�N�g��
	return �r���[�s��*/
	okmonn::Matrix4x4 GetViewMatrix(const okmonn::Vec3f& pos, const okmonn::Vec3f& target,
		const okmonn::Vec3f& up = okmonn::Vec3f(0.0f, 1.0f, 0.0f));
	/*�p�[�X�y�N�e�B�u�s��̎擾
	.����p���W�A���p�x
	.�A�X�y�N�g��
	.�ߒl
	.���l
	return �p�[�X�y�N�e�B�u�s��*/
	okmonn::Matrix4x4 GetPerspectiveMatrix(const float& fov, const float& aspect, const float& near_z, const float& far_z);
}

namespace okmonn {
	/*�������R�s�[
	.�R�s�[��o�b�t�@
	.�R�s�[���o�b�t�@
	.�R�s�[�T�C�Y*/
	template <typename T>
	void Copy(T* dst, const void* src, const std::uint32_t& copy_size) {
		for (std::uint32_t i = 0; i < copy_size / sizeof(T); ++i) {
			dst[i] = ((T*)src)[i];
		}
	}
	/*�ʓx�@�ɕϊ�
	.���W�A���p�x
	return �ʓx�@*/
	template <typename T>
	T ConvertArcDegree(const T& radian) {
		return radian * T(180.0) / std::acos(T(-1.0));
	}
	/*���W�A���p�x�ɕϊ�
	.�ʓx�@
	return ���W�A���p�x*/
	template <typename T>
	T ConvertRadian(const T& angle) {
		return angle * std::acos(T(-1.0)) / T(180.0);
	}
}

#endif
