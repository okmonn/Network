#ifndef DIRECTX_FENCE
#define DIRECTX_FENCE

#include "BaseClass.h"
#include <cstdint>

struct Device;
struct CommandQueue;

struct Fence :
	public BaseClass<ID3D12Fence1> {
public:
	/*�R���X�g���N�^
	.Device
	.CommandQueue*/
	Fence(const Device& device, const CommandQueue& command_queue);

public:
	/*�R�}���h���X�g�̑ҋ@*/
	void WaitForCommandExecution(void);

private:
	/*�t�F���X�̐���
	.Device*/
	void CreateFence(const Device& device);

private:
	/*CommandQueue*/
	const CommandQueue* command_queue;

private:
	/*�t�F���X�J�E���g*/
	std::uint64_t count;
};

#endif
