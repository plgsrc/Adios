#include "Cheats.hpp"

namespace CTRPluginFramework
{
	// ---------------------------------------------------------------
	// 未使用アイテムを表示
	// ---------------------------------------------------------------
	void DisableItemLocks(MenuEntry* e)
	{
		Keyboard key("", { "オン", "オフ" });
		int r = key.Open();

		if (r == 0)
		{
			Process::Write32(0x7238c0, 0xe1a00000);
			Process::Write32(0xad0250, 0x01000000);
		}
		else if (r == 1)
		{
			Process::Write32(0x7238c0, 0x0a000001);
			Process::Write32(0xad0250, 0x00000000);
		}
	}


	// ---------------------------------------------------------------
	// 置いても無くならない
	// ---------------------------------------------------------------
	void InfinityItem(MenuEntry* e)
	{
		Keyboard key("", { "オン", "オフ" });
		int r = key.Open();

		if (r == 0)
		{
			Process::Write32(0x0019C4D0, 0x00000000);
			Process::Write32(0x0019C42C, 0x00000000);
		}
		else if (r == 1)
		{
			Process::Write32(0x0019C4D0, 0xEB057D0A);
			Process::Write32(0x0019C42C, 0xEB057D33);
		}
	}


	// ---------------------------------------------------------------
	// 全て削除
	// ---------------------------------------------------------------
	void DeleteInvItems(MenuEntry* e)
	{
		if( (MessageBox("持ち物欄のアイテムをすべて削除しますか？"))() )
		{
			for( u8 slot = 0; slot < 16; slot++ )
				*Player::GetInvSlotAddr(slot) = 0x7FFE;
			
			OSD::SwapBuffers();
			(MessageBox(""))();
		}
	}


	// ---------------------------------------------------------------
	// 選択変更
	// ---------------------------------------------------------------
	std::vector<std::string> InvItemOption_opt =
	{
		"見せびらかす",
		"壁掛け & 床に飾る",
		"食べる",
		"手紙を書く"
	};
	void InvItemOption(MenuEntry *entry)
	{
		const u32 Offsets[4] = { 0xB9D8, 0xBFC8, 0xC150, 0xBA38 };
		const u32 Patches[4] = { 0x1A000012, 0x0A00001A, 0x0A000009, 0x1A000012 };
		const u32 NOP = 0xE1A00000;
		Keyboard key1(InvItemOption_opt);
		Keyboard key2("", { "追加", "削除" });
		
		int res1, res2;
		while( true )
		{
			OSD::SwapBuffers();
			res1 = key1.Open();
			if( res1 >= 0 && res1 <= 3 )
			{
				OSD::SwapBuffers();
				res2 = key2.Open();
				if( res2 >= 0 )
					Process::Write32( 0x190000 + Offsets[res1], res2 == 1 ? Patches[res1] : NOP );
			}
			else break;
		}
	}
	
	// ------------------------------------------------------
	// アイテム取得
	// ------------------------------------------------------
	extern bool AnySelecting;
	void TextToItem(MenuEntry *entry)
	{
		if ( Controller::IsKeysDown(X + DPadRight) && !AnySelecting )
		{
			u16 item;
			Keyboard k("アイテムID");
			if ( k.Open(item) == 0 ) *Player::GetInvSlotAddr(0) = item;
		}
	}
	
	// ------------------------------------------------------
	// 所持金変更
	// ------------------------------------------------------
	void ChangeWalletBell(MenuEntry *entry)
	{
		Keyboard k;
		k.IsHexadecimal(false);
		
		u32 addr = (*(u32*)0xAA914C) + 0x6F08;
		u32 money = DecryptACNLMoney( *(u32*)(addr) );
		
		if( k.Open( money, money ) == 0 )
			Process::Write32( addr, EncryptACNLMoney(money) );
	}
	
	
	
	
}


