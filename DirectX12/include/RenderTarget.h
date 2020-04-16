#ifndef DIRECTX_RENDER_TARGET
#define DIRECTX_RENDER_TARGET

#include "DescriptorHeap.h"
#include <vector>

struct Device;
struct CommandList;
struct SwapChain;

struct RenderTarget {
public:
	/*�R���X�g���N�^
	.Device
	.SwapChain*/
	RenderTarget(const Device& device, const SwapChain& swap_chain);

public:
	/*���݂̃��\�[�X�̎擾
	return ���\�[�X*/
	ID3D12Resource1* Get(void) const;
	/*���݂̃o�b�N�o�b�t�@�ԍ��̎擾
	return ���݂̃o�b�N�o�b�t�@*/
	std::uint32_t GetBackBufferIndex(void) const;

private:
	/*���\�[�X�̎擾
	.SwapChain*/
	void GetRsc(const SwapChain& swap_chain);
	/*RTV�̐���
	.Device*/
	void CreateRenderTargetView(const Device& device);

public:
	/*�q�[�v*/
	DescriptorHeap heap;

private:
	/*�X���b�v�`�F�C��*/
	const SwapChain* swap_chain;

private:
	/*���\�[�X*/
	std::vector<ComPtr<ID3D12Resource1>>rsc;
};

#endif
