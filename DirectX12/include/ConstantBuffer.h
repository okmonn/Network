#ifndef DIRECTX_CONSTANT_BUFFER
#define DIRECTX_CONSTANT_BUFFER

#include "DescriptorHeap.h"
#include <vector>

struct ConstantBuffer : 
	public BaseClass<ID3D12Resource1> {
public:
	/*�R���X�g���N�^*/
	ConstantBuffer();
	/*�R���X�g���N�^
	.Device
	.�f�[�^�T�C�Y*/
	ConstantBuffer(const Device& device, const std::uint32_t& data_size);

public:
	/*CBV�̐���
	.Device
	.�f�[�^�T�C�Y*/
	void CreateConstantBufferView(const Device& device, const std::uint32_t& data_size);
	/*�萔�f�[�^�̍X�V
	.�萔�f�[�^
	.�f�[�^�T�C�Y*/
	void UpDateConstant(const void* data, const std::uint32_t& data_size);

private:
	/*���\�[�X�̐���
	.Device
	.�f�[�^�T�C�Y*/
	void CreateRsc(const Device& device, const std::uint32_t& data_size);

public:
	/*�q�[�v*/
	DescriptorHeap heap;
};

#endif
