#ifndef OKMONN_STRING
#define OKMONN_STRING

#include <string>
#include <vector>

namespace okmonn {
	struct Code {
	public:
		/*�R���X�g���N�^*/
		Code();
		/*�R���X�g���N�^
		.�}���`�o�C�g������*/
		Code(const std::string& str);
		/*�R���X�g���N�^
		.���j�R�[�h������*/
		Code(const std::wstring& wstr);

	public:
		/*�}���`�o�C�g������̎擾
		return �}���`�o�C�g������*/
		std::string MultibyteCode(void) const;
		/*���j�R�[�h������
		return ���j�R�[�h������*/
		std::wstring UniCode(void) const;

		/*���Z�q�I�[�o�[���[�h*/
	public:
		void operator=(const Code& code);
		void operator=(const std::string& str);
		void operator=(const std::wstring& wstr);

	private:
		/*�i�[�o�b�t�@*/
		std::vector<std::uint8_t>buffer;
		/*������*/
		std::uint32_t word_num;
		/*���j�R�[�h�t���O*/
		std::uint8_t unicode;
	};
}

#endif
