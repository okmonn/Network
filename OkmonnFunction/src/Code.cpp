#include "../include/Code.h"
#include <cassert>

okmonn::Code::Code() : 
	word_num(0), unicode(0)
{
}

okmonn::Code::Code(const std::string& str)
{
	(*this) = str;
}

okmonn::Code::Code(const std::wstring& wstr)
{
	(*this) = wstr;
}

std::string okmonn::Code::MultibyteCode(void) const
{
	if (unicode == 1) {
		std::mbstate_t state{};
		size_t size = 0;
		const wchar_t* ptr = (const wchar_t*)buffer.data();
		auto error = wcsrtombs_s(&size, nullptr, 0, &ptr, 0, &state);
		assert(error == 0);
		std::vector<char>tmp(size);
		error = wcsrtombs_s(nullptr, tmp.data(), size, &ptr, size, &state);
		assert(error == 0);
		return std::string(tmp.data());
	}
	return std::string((char*)buffer.data());
}

std::wstring okmonn::Code::UniCode(void) const
{
	if (unicode == 0) {
		std::mbstate_t state{};
		size_t size = 0;
		const char* ptr = (const char*)buffer.data();
		auto error = mbsrtowcs_s(&size, nullptr, 0, &ptr, 0, &state);
		assert(error == 0);
		std::vector<wchar_t>tmp(size);
		error = mbsrtowcs_s(nullptr, tmp.data(), size, &ptr, size, &state);
		assert(error == 0);
		return std::wstring(tmp.data());
	}
	return std::wstring((wchar_t*)buffer.data());
}

void okmonn::Code::operator=(const Code& code)
{
	buffer   = code.buffer;
	word_num = code.word_num;
	unicode  = code.unicode;
}

void okmonn::Code::operator=(const std::string& str)
{
	buffer.resize(sizeof(char) * (str.size() + 1));
	buffer.shrink_to_fit();
	for (std::uint32_t i = 0; i < str.size(); ++i) {
		buffer[i] = str[i];
	}
	buffer[str.size()] = '\0';
	word_num = std::uint32_t(str.size());
	unicode  = 0;
}

void okmonn::Code::operator=(const std::wstring& wstr)
{
	buffer.resize(sizeof(wchar_t) * (wstr.size() + 1));
	buffer.shrink_to_fit();
	for (std::uint32_t i = 0; i < wstr.size(); ++i) {
		buffer[i] = (std::uint8_t)wstr[i];
	}
	buffer[wstr.size()] = L'\0';
	word_num = std::uint32_t(wstr.size());
	unicode  = 1;
}
