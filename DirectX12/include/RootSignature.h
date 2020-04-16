#ifndef DIRECTX_ROOT_SIGNATURE
#define DIRECTX_ROOT_SIGNATURE

#include "BaseClass.h"
#include <vector>

struct Device;

struct RootSignature :
	public BaseClass<ID3D12RootSignature> {
public:
	/*�R���X�g���N�^
	.Device
	.���[�g�p�����[�^���X�g
	.�T���v���[���X�g*/
	RootSignature(const Device& device,
		const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);

private:
	/*���[�g�V�O�l�`���̐���
	.Device
	.���[�g�p�����[�^���X�g
	.�T���v���[���X�g*/
	void CreateRootSignature(const Device& device,
		const std::vector<D3D12_ROOT_PARAMETER1>& param, const std::vector<D3D12_STATIC_SAMPLER_DESC>& sampler);
};

#endif
