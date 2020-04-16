#ifndef DIRECTX_DEPTH_STENCIL
#define DIRECTX_DEPTH_STENCIL

#include "DescriptorHeap.h"
#include "Vector.h"

struct DepthStencil :
	public BaseClass<ID3D12Resource1> {
public:
	/*�R���X�g���N�^*/
	DepthStencil();
	/*�R���X�g���N�^
	.Device
	.�E�B���h�E�T�C�Y*/
	DepthStencil(const Device& device, const okmonn::Vec2& window_size);

public:
	/*DSV�̐���
	.Device
	.�E�B���h�E�T�C�Y*/
	void CreateDepthStencilView(const Device& device, const okmonn::Vec2& window_size);

private:
	/*���\�[�X�̐���
	.Device
	.�E�B���h�E�T�C�Y*/
	void CreateRsc(const Device& device, const okmonn::Vec2& window_size);

public:
	/*�q�[�v*/
	DescriptorHeap heap;
};

#endif
