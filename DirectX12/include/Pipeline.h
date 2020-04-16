#ifndef DIRECTX_PIPELINE
#define DIRECTX_PIPELINE

#include "BaseClass.h"
#include <vector>

struct Device;
struct SwapChain;
struct RootSignature;

struct Pipeline :
	public BaseClass<ID3D12PipelineState> {
public:
	/*�R���X�g���N�^
	.Device
	.SwapChain
	.RootSignature
	.�V�F�[�_���
	.���͑������X�g
	.�g�|���W�[���
	.�[�x�t���O*/
	Pipeline(const Device& device, const SwapChain& swap_chain, 
		const RootSignature& root_signature, const std::vector<ID3DBlob*>& shader,
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology_type, const bool& depth_flag = false);

private:
	/*�p�C�v���C���̐���
	.Device
	.SwapChain
	.RootSignature
	.�V�F�[�_���
	.���͑������X�g
	.�g�|���W�[���
	.�[�x�t���O*/
	void CreatePipeline(const Device& device, const SwapChain& swap_chain,
		const RootSignature& root_signature, const std::vector<ID3DBlob*>& shader,
		const std::vector<D3D12_INPUT_ELEMENT_DESC>& input, const D3D12_PRIMITIVE_TOPOLOGY_TYPE& topology_type, const bool& depth_flag);
};

#endif
