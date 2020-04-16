#include "../include/Function.h"

okmonn::Matrix4x4 okmonn::GetViewMatrix(const okmonn::Vec3f& pos, const okmonn::Vec3f& target, const okmonn::Vec3f& up)
{
	okmonn::Vec3f zaxis = okmonn::Vec3f(target - pos).Normalization();
	okmonn::Vec3f xaxis = okmonn::Vec3f(okmonn::Cross(up, zaxis)).Normalization();
	okmonn::Vec3f yaxis = okmonn::Cross(zaxis, xaxis);

	return okmonn::Matrix4x4(okmonn::Vec4f(xaxis.x, xaxis.y, xaxis.z, -okmonn::Dot(xaxis, pos)),
							 okmonn::Vec4f(yaxis.x, yaxis.y, yaxis.z, -okmonn::Dot(yaxis, pos)),
							 okmonn::Vec4f(zaxis.x, zaxis.y, zaxis.z, -okmonn::Dot(zaxis, pos)),
							 okmonn::Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
}

okmonn::Matrix4x4 okmonn::GetPerspectiveMatrix(const float& fov, const float& aspect, const float& near_z, const float& far_z)
{
	float h = std::tan(fov / 2.0f);
	float w = h / aspect;

	return okmonn::Matrix4x4(okmonn::Vec4f(w, 0.0f, 0.0f, 0.0f),
							 okmonn::Vec4f(0.0f, h, 0.0f, 0.0f),
							 okmonn::Vec4f(0.0f, 0.0f, far_z / (far_z - near_z), (-near_z * far_z) / (far_z - near_z)),
							 okmonn::Vec4f(0.0f, 0.0f, 1.0f, 0.0f));
}
