#ifndef DIRECTX_DESCRIPTOR_HEAP
#define DIRECTX_DESCRIPTOR_HEAP

#include "BaseClass.h"
#include <cstdint>

struct Device;
struct CommandList;

struct DescriptorHeap :
	public BaseClass<ID3D12DescriptorHeap> {
public:
	/*�R���X�g���N�^*/
	DescriptorHeap();

public:
	/*�q�[�v�̐���
	.Device
	.�q�[�v���
	.���\�[�X��
	.�V�F�[�_���p�t���O*/
	void CreateHeap(const Device& device, const D3D12_DESCRIPTOR_HEAP_TYPE& type, const std::uint32_t& rsc_num, const bool& shader_use_flag = false);
};

#endif
