#pragma once

namespace CTRPluginFramework
{
	class NPC
	{
	public:
		static bool		GetCoordinatesRef(u32 npc, float &X, float &Z);
	};

	class IDList
	{
	public:
		static std::string GetItemName(u32 ID);
		static std::string GetBuildingName(u8 ID);
		static std::string GetSnakeName(u16 ID);
		static std::string GetRoomName(u8 ID);
	};

	class Item
	{
	public:
		static u8		GetBehaviour(u32 *pItem);
		static void		hasTreeBehaviour(u32 *pItem);
	};

	class Camera
	{
	public:
		static u32		GetInstance();
		static void		AddToX(float val);
		static void		AddToY(float val);
		static void		AddToZ(float val);
		static void		AddToYRotation(u16 val);
	};

	class ItemSequence
	{
	public:
		static void		init(u32 defaultPtr);
		static u32		*Next();
		static u32		PeekNext();
		static bool		openKB();
		static void		Switch(bool enable);
		static bool		Enabled();
	};

	
}




