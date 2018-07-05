#pragma once
#include <Windows.h>

/* �E�B���h�E */
class Window
{
public:
	// �R���X�g���N�^
	Window(INT x, INT y);
	// �f�X�g���N�^
	~Window();

	// �E�B���h�E�v���V�[�W��
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// �E�B���h�E�n���h���̎擾
	constexpr HWND GetHandle(void) const {
		return handle;
	}

	// �E�B���h�E�T�C�Y�̎擾
	constexpr INT GetX(void) const {
		return x;
	}
	// �E�B���h�E�T�C�Y�̎擾
	constexpr INT GetY(void) const {
		return y;
	}

private:
	// �E�B���h�E�v�f�̃Z�b�g
	void SetWindow(void);

	// �E�B���h�E�T�C�Y�̃Z�b�g
	void SetWindowSize(void);

	// �E�B���h�E�̐���
	void Create(void);


	// �E�B���h�E�n���h��
	HWND handle;

	// �E�B���h�E
	WNDCLASSEX window;

	// �E�B���h�E�T�C�Y
	RECT rect;

	// X�T�C�Y
	INT x;

	// Y�T�C�Y
	INT y;
};
