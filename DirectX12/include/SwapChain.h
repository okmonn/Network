#ifndef DIRECTX_SWAP_CHAIN
#define DIRECTX_SWAP_CHAIN

#include "BaseClass.h"
#include <cstdint>

class Window;
struct CommandQueue;

struct SwapChain :
	public BaseClass<IDXGISwapChain4> {
public:
	/*�R���X�g���N�^
	.Window
	.CommandQueue
	.�o�b�N�o�b�t�@��*/
	SwapChain(const Window& window, const CommandQueue& command_queue,
		const std::uint32_t& back_buffer_num = 2);

public:
	/*�\�E���o�b�t�@�̐؂�ւ�*/
	void Present(void) const;

public:
	/*�o�b�N�o�b�t�@���̎擾
	return �o�b�N�o�b�t�@��*/
	std::uint32_t GetBackBufferNum(void) const;

private:
	/*�X���b�v�`�F�C���̐���
	.Window
	.CommandQueue*/
	void CreateSwapChain(const Window& window, const CommandQueue& command_queue);

private:
	/*�o�b�N�o�b�t�@��*/
	std::uint32_t back_buffer_num;
};

#endif
