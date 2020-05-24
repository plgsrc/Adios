#include "Cheats.hpp"
#include "Game/Items.h"
#include "kanji.h"

#define		WAIT_MS		100
#define		T2C_DEBUG

namespace CTRPluginFramework
{
	extern bool TouchDrop_Enabled;
	extern const std::vector<std::string> ActionNames;
	bool AnySelecting;
	
	// 天気変更
	std::vector<std::string> Weathers =
	{
		"晴れ(雲なし)",
		"晴れ(雲あり)",
		"くもり空",
		"雨(小)",
		"雨",
		"雪(小)",
		"雪",
	};
	
	void TextToCheats(MenuEntry *entry)
	{
		static Clock clock;
		static std::vector<std::string> Items;
		
		if( !Chat::IsOpened() )
		{
		}
		
		if( Controller::IsKeyDown(B) )
		{
			if( Controller::IsKeyPressed(R) || (Controller::IsKeyDown(R) && clock.HasTimePassed(Milliseconds(WAIT_MS))) )
			{
				
				
			}
			
			else if( Controller::IsKeyPressed(R) || (Controller::IsKeyDown(R) && clock.HasTimePassed(Milliseconds(WAIT_MS))) )
			{
				
				
			}
		}
		
		
		
	}
	
	
	
	
	
	
	
}


