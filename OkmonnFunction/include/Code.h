#ifndef OKMONN_STRING
#define OKMONN_STRING

#include <string>
#include <vector>

namespace okmonn {
	struct Code {
	public:
		/*コンストラクタ*/
		Code();
		/*コンストラクタ
		.マルチバイト文字列*/
		Code(const std::string& str);
		/*コンストラクタ
		.ユニコード文字列*/
		Code(const std::wstring& wstr);

	public:
		/*マルチバイト文字列の取得
		return マルチバイト文字列*/
		std::string MultibyteCode(void) const;
		/*ユニコード文字列
		return ユニコード文字列*/
		std::wstring UniCode(void) const;

		/*演算子オーバーロード*/
	public:
		void operator=(const Code& code);
		void operator=(const std::string& str);
		void operator=(const std::wstring& wstr);

	private:
		/*格納バッファ*/
		std::vector<std::uint8_t>buffer;
		/*文字数*/
		std::uint32_t word_num;
		/*ユニコードフラグ*/
		std::uint8_t unicode;
	};
}

#endif
