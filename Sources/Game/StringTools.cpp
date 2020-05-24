#include "CTRPluginFramework.hpp"
#include "Game.hpp"

namespace CTRPluginFramework
{
	// 指定したアドレスからUTF16文字列をUTF8文字列に変換して取得します
	std::string StringTools::GetString(u32 address, const u32& length)
	{
		std::string r;
		Process::ReadString(address, r, length * 2, StringFormat::Utf16);
		return r;
	}
	
	// UTF8文字列を指定したアドレスに書き込みます
	void StringTools::SetString(u32 address, const std::string& str)
	{
		Process::WriteString(address, str, StringFormat::Utf16);
	}
	
	// 文字列をUTF16に変換します
	void StringTools::ToUTF16(const std::string& str, u16 out[])
	{
		Process::WriteString((u32)out, str, StringFormat::Utf16);
	}

	// UTF8をUTF32に変換します
	void StringTools::ToUTF32(const std::string& str, u32 out[])
	{
		Process::WriteString((u32)out, str, StringFormat::Utf32);
	}
}

