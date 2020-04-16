#ifndef DIRECTX_COMMAND_QUEUE
#define DIRECTX_COMMAND_QUEUE

#include "BaseClass.h"
#include <vector>

struct Device;
struct CommandList;

struct CommandQueue :
	public BaseClass<ID3D12CommandQueue> {
public:
	/*�R���X�g���N�^
	.Device
	.�R�}���h�^�C�v*/
	CommandQueue(const Device& device, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

public:
	/*�R�}���h�̎��s
	.CommandList�̔z��*/
	void ExecutionCommand(const std::vector<CommandList*>& command_list);

private:
	/*�R�}���h�L���[�̐���
	.Device
	.�R�}���h�^�C�v*/
	void CreateCommandQueue(const Device& device, const D3D12_COMMAND_LIST_TYPE& type);
};

#endif
