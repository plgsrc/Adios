#pragma once

namespace CTRPluginFramework
{
	class GameHelper
	{
	public:
		static void		WriteItem(u32 ID);
		static u32		PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool isInside, bool u1, bool u2, bool u3, bool u4);
		static u8		GetOnlinePlayerIndex();
		static u8		GetActualPlayerIndex();
		static u8		GetOnlinePlayerCount();
		static bool		Warp0();
		static bool		ArriveAtIsland(u32 u0 = 0x31FE4ECC);
		static bool		ArriveAtTourdesk(u32 u0 = 0x31FE4ECC);
		static bool		ArriveAtTown(u32 u0 = 0x31FE4ECC);
		static bool		LeaveIsland(u32 u0 = 0x31FE4ECC);
		static u32		*GetItemAtWorldCoords(u32 MapPtr, u32 x, u32 y, bool u0);
		static u32		GetCurrentMap();
		static void		PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID);
		static u32		APressAction(u32 x, u32 y, u32 ID);
		static void		DropIDSwitch(u32 itemToRemove, u32 itemToShow, u32 itemToPlace, u8 u0, u8 playerIndex, u8 wX, u8 wY, u8 dropID, u8 u1, u8 u2, u8 u3, u8 u4, u8 roomID);
		static void		playSound(u32 playerIndex, u32 ID);
		static bool		dropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1);
		static bool		PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID);
		static bool		RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything);
		static void		Particles(u32 particleID, float *floats, u32 u0 = 0x96FC06, u32 u1 = 0xADF870);
		static void		clearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4 = 4);
		static u32		createLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt = 1);
		static u32		GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID = 0xA5);
		static void		TrampleAt(u8 wX, u8 wY);
		static float	*WorldCoordsToCoords(u8 wX, u8 wY, float res[3]);
	};
	
	
	
}




