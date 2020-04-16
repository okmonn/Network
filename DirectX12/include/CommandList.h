#ifndef DIRECTX_COMMAND_LIST
#define DIRECTX_COMMAND_LIST

#include "BaseClass.h"
#include "Vector.h"

struct Device;
struct CommandAllocator;
struct RenderTarget;
struct DepthStencil;
struct RootSignature;
struct Pipeline;
struct VertexBuffer;
struct IndexBuffer;
struct ConstantBuffer;
struct ResourceBuffer;

struct CommandList :
	public BaseClass<ID3D12GraphicsCommandList5> {
public:
	/*�R���X�g���N�^
	.Device
	.�R�}���h�^�C�v*/
	CommandList(const Device& device, const D3D12_COMMAND_LIST_TYPE& type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT);

public:
	/*���Z�b�g
	.CommandAllocator*/
	void Reset(const CommandAllocator& command_allocator) const;
	/*�N���[�Y*/
	void Close(void) const;
	/*�r���[�|�[�g�̐ݒ�
	.�E�B���h�E�T�C�Y*/
	void SetViewport(const okmonn::Vec2& window_size) const;
	/*�V�U�[�̐ݒ�
	.�E�B���h�E�T�C�Y*/
	void SetScissor(const okmonn::Vec2& window_size) const;
	/*���\�[�X�o���A
	.���\�[�X
	.�J�ڑO���\�[�X���
	.�J�ڌナ�\�[�X���*/
	void SetResourceTransitionBarrier(ID3D12Resource1* rsc, 
		const D3D12_RESOURCE_STATES& befor, const D3D12_RESOURCE_STATES& after);
	/*�����_�[�^�[�Q�b�g�̃N���A
	.RenderTarget
	.�N���A�F
	.DepthStencil*/
	void ClearRenderTarget(const RenderTarget& render_target, const float clear_color[4], const DepthStencil* depth_stencil);
	/*�f�v�X�X�e���V���̃N���A
	.DepthStencil*/
	void ClearDepthStencil(const DepthStencil& depth_stencil);
	/*�`��p���[�g�V�O�l�`���̃Z�b�g
	.RootSignature*/
	void SetGraphicRootSignature(const RootSignature& root_signature);
	/*�p�C�v���C���̃Z�b�g
	.Pipeline*/
	void SetPipeline(const Pipeline& pipeline);
	/*�`��p�萔�o�b�t�@�̃Z�b�g
	.ConstantBuffer
	.���[�g�p�����[�^�ԍ�
	.���\�[�X�ԍ�*/
	void SetGraphicConstantBuffer(const ConstantBuffer& constant_buffer, const std::uint32_t& root_param_index, const std::uint32_t& rsc_index = 0);
	/*�`��p���\�[�X�o�b�t�@�̃Z�b�g
	.ResourceBuffer
	.���[�g�p�����[�^�ԍ�*/
	void SetGraphicResourceBuffer(const ResourceBuffer& resource_buffer, const std::uint32_t& root_param_index);
	/*���_�o�b�t�@����ɕ`��R�}���h�𔭍s
	.VertexBuffer
	.���_��
	.�g�|���W�[���
	.�C���X�^���X��*/
	void DrawCall(const VertexBuffer& vertex_buffer, const std::uint32_t& vertex_num, 
		const D3D12_PRIMITIVE_TOPOLOGY& topology_type, const std::uint32_t& instance_num = 1);
	/*�C���f�b�N�X�o�b�t�@����ɕ`��R�}���h�𔭍s
	.VertexBuffer
	.IndexBuffer
	.���_��
	.�C���f�b�N�X��
	.�g�|���W�[���
	.�C���f�b�N�X�o�b�t�@�J�n�I�t�Z�b�g
	.�C���X�^���X��*/
	void DrawCall(const VertexBuffer& vertex_buffer, const IndexBuffer& index_buffer, 
		const std::uint32_t& vertex_num, const std::uint32_t& index_num,
		const D3D12_PRIMITIVE_TOPOLOGY& topology_type, const std::uint32_t& index_buffer_offset = 0, const std::uint32_t& instance_num = 1);

private:
	/*�R�}���h���X�g�̐���
	.Device
	.�R�}���h�^�C�v*/
	void CreateCommandList(const Device& device, const D3D12_COMMAND_LIST_TYPE& type);
};

#endif
