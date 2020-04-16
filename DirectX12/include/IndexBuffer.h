#ifndef DIRECTX_INDEX_BUFFER
#define DIRECTX_INDEX_BUFFER

#include "BaseClass.h"
#include <cstdint>

struct Device;

struct IndexBuffer :
	public BaseClass<ID3D12Resource1> {
public:
	/*�R���X�g���N�^
	.Device
	.�o�b�t�@�T�C�Y*/
	IndexBuffer(const Device& device, const std::uint32_t& buffer_size);

public:
	/*�C���f�b�N�X�f�[�^�̍X�V
	.�C���f�b�N�X�f�[�^*/
	void UpDateIndex(const void* data);

private:
	/*���\�[�X�̐���
	.Device
	.�o�b�t�@�T�C�Y*/
	void CraeteRsc(const Device& device, const std::uint32_t& buffer_size);
};

#endif
