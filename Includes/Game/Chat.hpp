#pragma once

namespace CTRPluginFramework
{
	class Chat
	{
	public:
		static bool IsOpened();
		static void Write(std::string str);
		static void Writeu(const u16 StrArray[], int index);
		static std::string Read();
		static void Readu(u16 output[], int index, int size = 0);
		static std::string GetSelectedString();
		static std::string GetUnselectedString(int right = -1);
		static void Change(int mode);
		static void Send();
		static void Clear();
		
	private:
		static int lenUtf8(std::string &str);
		static int lenByte(unsigned char c);
		static u32 _get_text_buffer_addr();
		static bool _isTown();
		static void _write_length(u8);
		static u8 _read_length();
		static u32 len_addr();
		
	};
	
	
}




