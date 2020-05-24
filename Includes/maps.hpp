#pragma once

namespace CTRPluginFramework
{
	struct NAME_VALUE_MAP
	{
		const char	*name;
		const u32	value;
	};
	
	struct BADGE_MAP
	{
		const u32	hex;
		const u32	hex2;
		const u32	hex3;
		const u32	hex4;
		const u32	hex5;
		const u32	hex6;
		const char	*name;
	};
	
	// UnicodeNotify
	struct UnicodeNotify_Info
	{
		std::string		text;
		Clock			clock;
		Color			fore;
		Color			back;
	};
	
	struct Building
	{
		u16 id;
		u8 x;
		u8 y;
	};

	struct Island
	{
		Building b[2];
		u8 acres[16];
	};
	
	struct ITEM_MAP
	{
		const u16	code;	// アイテムID
		const char	*name1; // 表示用
		const char	*name2; // 検索用
	};
	
	struct dropInfo
	{
		u32 itemToRemove;
		u32 itemToShow;
		u32 itemToPlace;
		u8 unknown0;
		u8 playerindex;
		u8 wX;
		u8 wY;
		u8 dropID;
		u8 unknown1;
		u8 unknown2;
		u8 unknown3;
		u8 unknown4;
		u8 roomID;
	};

	struct dropPkt
	{
		u32 replaceID;
		u32 placeID;
		u32 showID;
		u8 combinedDropIDPlayerIndex;
		u8 processFlags;
		u8 paramFlags;
		u8 flags;
		u8 u0;
		u8 u1;
		u8 roomID;
		u8 wX;
		u8 wY;
		u8 u2;
	};

	struct TramplePkt
	{
		u32 item;
		u8 roomID;
		u8 wX;
		u8 wY;
		u8 u0;
	};
	
	
	
	
}


