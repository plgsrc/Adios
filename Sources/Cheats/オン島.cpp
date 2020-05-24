#include "Cheats.hpp"

namespace CTRPluginFramework
{
	// ------------------------------------------------------
	// 島のメダル設定
	// ------------------------------------------------------
	void ChangeIslandMedal(MenuEntry *entry)
	{
		Keyboard k("メダルの枚数");
		k.IsHexadecimal(false);
		u16 medal;
		
		if( k.Open(medal) == 0 )
			Process::Write64( (*(u32*)0xAA914C) + 0x6b9c, EncryptACNLMoney(medal) );
	}
	
}


