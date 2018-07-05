#pragma once
#include <Windows.h>

/* ウィンドウ */
class Window
{
public:
	// コンストラクタ
	Window(INT x, INT y);
	// デストラクタ
	~Window();

	// ウィンドウプロシージャ
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// ウィンドウハンドルの取得
	constexpr HWND GetHandle(void) const {
		return handle;
	}

	// ウィンドウサイズの取得
	constexpr INT GetX(void) const {
		return x;
	}
	// ウィンドウサイズの取得
	constexpr INT GetY(void) const {
		return y;
	}

private:
	// ウィンドウ要素のセット
	void SetWindow(void);

	// ウィンドウサイズのセット
	void SetWindowSize(void);

	// ウィンドウの生成
	void Create(void);


	// ウィンドウハンドル
	HWND handle;

	// ウィンドウ
	WNDCLASSEX window;

	// ウィンドウサイズ
	RECT rect;

	// Xサイズ
	INT x;

	// Yサイズ
	INT y;
};
