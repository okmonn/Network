#ifndef DIRECTX_RESOURCE_BUFFER
#define DIRECTX_RESOURCE_BUFFER

#include "DescriptorHeap.h"

struct Device;

struct ResourceBuffer :
	public BaseClass<ID3D12Resource1> {
public:
	/*�R���X�g���N�^*/
	ResourceBuffer();
	/*�R���X�g���N�^
	.Device
	.����
	.�c��*/
	ResourceBuffer(const Device& device, const std::uint32_t& width, const std::uint32_t& height);

public:
	/*SRV�̐���
	.Device
	.����
	.�c��*/
	void CreateShaderResourceView(const Device& device, const std::uint32_t& width, const std::uint32_t& height);
	/*���\�[�X�f�[�^�̍X�V
	.���\�[�X�f�[�^*/
	void UpDateResource(const void* data);

private:
	/*���\�[�X�̐���
	.Device
	.����
	.�c��*/
	void CreateRsc(const Device& device, const std::uint32_t& width, const std::uint32_t& height);

public:
	/*�q�[�v*/
	DescriptorHeap heap;
};

#endif
