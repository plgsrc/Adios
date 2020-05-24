#include "CTRPluginFramework.hpp"
#include "maps.hpp"
#include "Game/Items.h"
#include "Helpers.hpp"
#include <string>
#include <vector>

namespace CTRPluginFramework
{
	// とび森通貨を複合化する
	u32 DecryptACNLMoney(u64 money)
    {
        // Unpack 64-bit value into (u32, u16, u8, u8) values.
        u32 enc = (money & 0xFFFFFFFF);
        u16 adjust = ((money >> 32) & 0xFFFF);
        u8  shift_val = ((money >> 48) & 0xFF);
        u8  chk = ((money >> 56) & 0xFF);

        // Validate 8-bit checksum
        if ((((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF) != chk) return 0;
        
        u8  left_shift = ((0x1C - shift_val) & 0xFF);
        u8  right_shift = 0x20 - left_shift;

        // Handle error case: Invalid shift value.
        if (left_shift >= 0x20)
        {
            return 0 + (enc << right_shift) - (adjust + 0x8F187432);
        }

        // This case should occur for all game-generated values.
        return (enc << left_shift) + (enc >> right_shift) - (adjust + 0x8F187432);
    }
	
	// 数値をとび森通貨の16進数値に暗号化する
    u64 EncryptACNLMoney(int dec)
    {
        // Make a new RNG
        u16 adjust = Utils::Random(0, 0x10000);
        u8  shift_val = Utils::Random(0, 0x1A);

        // Encipher value
        u32 enc = dec + adjust + 0x8F187432;
        enc = (enc >> (0x1C - shift_val)) + (enc << (shift_val + 4));

        // Calculate Checksum
        u8  chk = (((enc >> 0) + (enc >> 8) + (enc >> 16) + (enc >> 24) + 0xBA) & 0xFF);

        // Pack result
        return ((u64)enc << 0) | ((u64)adjust << 32) | ((u64)shift_val << 48) | ((u64)chk << 56);
    }
	
	// カタカナをひらがなにする
	void k2h( std::string &str )
	{
		u16 Utf16_Str[200] = { 0 };
		
		Process::WriteString( (u32)Utf16_Str, str, StringFormat::Utf16 );
		
		for (u16 &moji : Utf16_Str)
			if ( moji >= 0x30a1 && moji <= 0x30f3 ) moji -= 0x60;
		
		Process::ReadString( (u32)Utf16_Str, str, 200, StringFormat::Utf16 );
	}
	
	// ---------------------------------------------------------------
	// アイテム検索
	// ～アイテム一覧の中から検索して、一致した場合はoutputに追加します。
	// ・str	: 探す文字列
	// ・output	: 追加されるベクター配列(ITEM_MAP)
	// ---------------------------------------------------------------
	void ItemSearch(const std::string &str, std::vector<ITEM_MAP> &output)
	{
		std::string tmp;
		for ( const ITEM_MAP &i: AllItems )
		{
			tmp = i.name2;
			k2h(tmp);
			
			if ( tmp.find(str) != std::string::npos )
				output.push_back(i);
		}
	}
	
	// ---------------------------------------------------------------
	// Unciode Notify
	// ---------------------------------------------------------------
	std::vector<UnicodeNotify_Info>	UN_Notifys;
	const u32						Unotify_Max = 10;
	void UnicodeNotify( const std::string &text, Color fore, Color back )
	{
		if ( UN_Notifys.size() >= Unotify_Max )
			UN_Notifys.pop_back();
		
		UnicodeNotify_Info info;
		info.text = text;
		info.clock.Restart();
		info.fore = fore;
		info.back = back;
		
		UN_Notifys.insert( UN_Notifys.begin(), info );
	}
	bool UniNotify_Callback( const Screen &scr )
	{
		if ( ! scr.IsTop ) return false;
		if ( UN_Notifys.empty() ) return false;
		
		int index = 0;
		for ( const UnicodeNotify_Info &info : UN_Notifys )
		{
			// Draw
			scr.DrawRect(398 - OSD::GetTextWidth(true, info.text), 220 - 18 * index, OSD::GetTextWidth(true, info.text), 16, info.back);
			scr.DrawSysfont(info.text, 398 - OSD::GetTextWidth(true, info.text), 220 - 18 * index, info.fore);
			
			// delete if time passed
			if ( info.clock.HasTimePassed(Seconds(5)) )
				UN_Notifys.erase(UN_Notifys.begin() + index);
			
			index++;
		}
		
		return true;
	}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}







