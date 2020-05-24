#pragma once

namespace CTRPluginFramework
{
	class StringTools
	{
	public:
		/**	指定したアドレスからUTF16文字列をUTF8文字列に変換して取得します
		 *
		 *	@param[address]	UTF16文字列の先頭アドレス
		 *	@param[length]	取り出す文字数
		 *
		 *	@returns		取り出された文字列(UTF8)
		 */
		static std::string GetString(u32 address, const u32& length);

		/**	UTF8文字列を指定したアドレスに書き込みます
		 *
		 *	@param[address]	文字列の先頭位置とするアドレス
		 *	@param[str]		書き込む文字列(UTF8)
		 */
		static void SetString(u32 address, const std::string& str);

		/**	文字列をUTF16に変換します
		 *
		 *	@param[str]		UTF16にしたい文字列
		 *	@param[out]		出力先( u16配列 )
		 */
		static void ToUTF16(const std::string& str, u16 out[]);

		/**	UTF8をUTF32に変換します
		 *
		 *	@param[str]		UTF32にしたい文字列(UTF8)
		 *	@param[out]		出力先( u32配列 )
		 */
		static void ToUTF32(const std::string& str, u32 out[]);
		
		
	};
}



