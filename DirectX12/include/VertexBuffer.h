#ifndef DIRECTX_VERTEX_BUFFER
#define DIRECTX_VERTEX_BUFFER

#include "../include/BaseClass.h"
#include <cstdint>

struct Device;

struct VertexBuffer : 
	public BaseClass<ID3D12Resource1> {
public:
	/*�R���X�g���N�^
	.Device
	.�o�b�t�@�T�C�Y*/
	VertexBuffer(const Device& device, const std::uint32_t& buffer_size);

public:
	/*���_�f�[�^�̍X�V
	.���_�f�[�^*/
	void UpDateVertex(const void* vertex_data);

private:
	/*���\�[�X�̐���
	.Device
	.�o�b�t�@�T�C�Y*/
	void CreateRsc(const Device& device, const std::uint32_t& buffer_size);
};

#endif
