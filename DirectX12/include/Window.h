#ifndef DIRECTX_WINDOW
#define DIRECTX_WINDOW

#include <Vector.h>

struct HWND__;

class Window {
public:
	/*�R���X�g���N�^
	.�E�B���h�E�T�C�Y
	.�X�N���[�����W*/
	Window(const okmonn::Vec2& window_size, const okmonn::Vec2& screen_pos = 0x80000000);
	/*�f�X�g���N�^*/
	~Window();

public:
	/*�E�B���h�E�n���h���̎擾
	return �E�B���h�E�n���h��*/
	HWND__* Get(void) const;
	/*�X�N���[�����W�̎擾
	return �X�N���[�����W*/
	okmonn::Vec2 GetScreenPos(void) const;
	/*�E�B���h�E�T�C�Y�̎擾
	return �E�B���h�E�T�C�Y*/
	okmonn::Vec2 GetWindowSize(void) const;

public:
	HWND__* operator->(void) const;
	HWND__** operator&(void);

private:
	/*�E�B���h�E�n���h���̐���*/
	void CreateWindowHandle(void);

private:
	/*�E�B���h�E�n���h��*/
	void* handle;
	/*�X�N���[�����W*/
	okmonn::Vec2 screen_pos;
	/*�E�B���h�E�T�C�Y*/
	okmonn::Vec2 window_size;
};

#endif
