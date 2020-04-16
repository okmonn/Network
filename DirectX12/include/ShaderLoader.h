#ifndef DIRECTX_SHADER_LOADER
#define DIRECTX_SHADER_LOADER

#include "BaseClass.h"
#include <string>

struct ShaderLoader :
	public BaseClass<ID3DBlob> {
public:
	/*�R���X�g���N�^
	.�t�@�C���p�X
	.�G���g���[�֐���
	.�V�F�[�_���f��*/
	ShaderLoader(const std::string& file_path, 
		const std::string& entry_func_name, const std::string& shader_model);

private:
	/*�V�F�[�_�̃R���p�C��
	.�t�@�C���p�X
	.�G���g���[�֐���
	.�V�F�[�_���f��*/
	void ShaderCompile(const std::string& file_path,
		const std::string& entry_func_name, const std::string& shader_model);
};

#endif
