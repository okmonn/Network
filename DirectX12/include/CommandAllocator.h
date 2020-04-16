#ifndef DIRECTX_COMMAND_ALLOCATOR
#define DIRECTX_COMMAND_ALLOCATOR

#include "BaseClass.h"

struct Device;

struct CommandAllocator : 
	public BaseClass<ID3D12CommandAllocator> {
public:
	/*�R���X�g���N�^
	.Device
	.�R�}���h�^�C�v*/
	CommandAllocator(const Device& device, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

public:
	/*���Z�b�g*/
	void Reset(void) const;

private:
	/*�R�}���h�A���P�[�^�̐���
	.Device
	.�R�}���h�^�C�v*/
	void CreateCommandAllocator(const Device& device, const D3D12_COMMAND_LIST_TYPE& type);
};

#endif
