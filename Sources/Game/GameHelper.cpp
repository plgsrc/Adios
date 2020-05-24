#include "CTRPluginFramework.hpp"
#include "CallFuncWrapper.hpp"
#include "strings.hpp"
#include "Address.h"
#include "Game.hpp"
#include "maps.hpp"

namespace CTRPluginFramework
{
	extern bool useDropChain;
	
	void GameHelper::WriteItem(u32 ID)
	{
		
	}
	
	u32 GameHelper::PlaceItem(u8 ID, u32 *ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4)
	{
		return (CallFuncWrapper(0x59E5B4))(ID, (u32)ItemToReplace, (u32)ItemToPlace, (u32)ItemToShow, worldx, worldy, u0, u1, u2, u3, u4);
	}
	
	u8 GameHelper::GetOnlinePlayerIndex()
	{
		return (CallFuncWrapper(USA0x00305EF0))();
	}
	u8 GameHelper::GetActualPlayerIndex()
	{
		u8 index = *(u8 *)((*(u32 *)0x94D644) + 0x13268);
		if (index >= 4) return 0;
		else return index;
	}
	u8 GameHelper::GetOnlinePlayerCount()
	{
		return (CallFuncWrapper(USA0x0075EFF8))(*(u32 *)0x94D644);
	}
	bool GameHelper::Warp0()
	{
		return (CallFuncWrapper(USA0x0061E708))();
	}
	bool GameHelper::ArriveAtIsland(u32 u0)
	{
		return (CallFuncWrapper( USA0x0061E8BC ))(u0);
	}
	bool GameHelper::ArriveAtTourdesk(u32 u0)
	{
		return (CallFuncWrapper( USA0x0061F81C ))(u0);
	}
	bool GameHelper::ArriveAtTown(u32 u0)
	{
		return (CallFuncWrapper( USA0x0061F258 ))(u0);
	}
	bool GameHelper::LeaveIsland(u32 u0)
	{
		return (CallFuncWrapper( USA0x0061E954 ))(u0);
	}
	u32 *GameHelper::GetItemAtWorldCoords(u32 MapPtr, u32 x, u32 y, bool u0)
	{
		return (u32 *) (CallFuncWrapper(USA0x002FEF9C))(MapPtr, x, y, u0);
	}
	u32 GameHelper::GetCurrentMap()
	{
		return (CallFuncWrapper(USA0x006A690C))();
	}
	void GameHelper::PlaceBuildingUpdateCollisions(u32 x, u32 y, u16 buildingID)
	{
		(CallFuncWrapper( USA0x002425D8 ))(x, y, buildingID);
	}
	u32 GameHelper::APressAction(u32 x, u32 y, u32 ID)
	{
		return (CallFuncWrapper( USA0x00597154 ))(x, y, ID);
	}
	void GameHelper::DropIDSwitch(u32 itemToRemove, u32 itemToShow, u32 itemToPlace, u8 u0, u8 playerIndex, u8 wX, u8 wY, u8 dropID, u8 u1, u8 u2, u8 u3, u8 u4, u8 roomID)
	{
		dropInfo obj{ itemToRemove, itemToShow, itemToPlace, u0, playerIndex, wX, wY, dropID, u1, u2, u3, u4, roomID };
		(CallFuncWrapper( USA0x005A22F4 ))((u32)&obj);
	}
	void GameHelper::playSound(u32 playerIndex, u32 ID)
	{
		u32 ptr = (CallFuncWrapper( USA0x005C119C ))(playerIndex); // get playerObj + 0x5EC
		if (ptr != 0) (CallFuncWrapper( USA0x002505E0 ))(ptr, ID, 1);
	}
	bool GameHelper::dropCheck(u32 *wX, u32 *wY, u32 *u0, bool disallowConcrete, bool u1)
	{
		return (bool) (CallFuncWrapper( USA0x005990AC ))((u32)wX, (u32)wY, (u32)u0, disallowConcrete, u1);
	}
	
	void pseudoType0x21(dropPkt *pkt)
	{
		dropPkt *drop = new dropPkt();
		u32 x, y;
		s8 diffX, diffY;
		u32 *item;

		drop->replaceID = 0x71;
		drop->placeID = pkt->placeID;
		drop->showID = 0x71;
		drop->paramFlags = 0;
		drop->processFlags = 0;
		drop->u0 = 0;
		drop->u1 = 0;
		drop->u2 = 0;
		drop->roomID = pkt->roomID;
		drop->wX = pkt->wX;
		drop->wY = pkt->wY;
		drop->combinedDropIDPlayerIndex = GameHelper::GetOnlinePlayerIndex() & 3 | 0x84;
		drop->processFlags = drop->processFlags & 0xC3 | (GameHelper::GetOnlinePlayerCount() > 1 ? 0x1C : 0xC);
		drop->flags = 0;
		Player::GetWorldCoords(&x, &y, 4, 1);
		pkt->wX = x;
		pkt->wY = y;
		pkt->replaceID = 0x71;
		item = GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0);
		if (item == nullptr) return;
		pkt->placeID = *item;
		pkt->showID = drop->placeID == 0x7FFE ? 0x2001 : drop->placeID;
		pkt->paramFlags = 0x8;
		diffX = drop->wX - x;
		diffY = drop->wY - y;
		pkt->flags = (((diffX + 4) & 0xF) | (((diffY + 4) & 0xF) << 4));
		pkt->u0 = 0;
		pkt->u1 = 0;
		pkt->u2 = 0;
		(CallFuncWrapper( USA0x0059FFC0 ))((u32)drop);
		return;
	}
	
	bool GameHelper::PlaceItemWrapper(u8 ID, u32 ItemToReplace, u32 *ItemToPlace, u32 *ItemToShow, u8 worldx, u8 worldy, bool u0, bool u1, bool u2, bool u3, bool u4, u8 waitAnim, u8 roomID)
	{
		static Hook h;
		float coords[3];
		u32 crashPreventItem = Player::GetLocation() != 0xFF ? 0x2001 : 0x80007FFE;
		u8 autoWaitAnim = waitAnim;
		u32 player = Player::GetInstance(4, 1);
		u32 *pItemAtCoords = GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), worldx, worldy, 0);
		u32 *actualItemToPlace = useDropChain ? ItemSequence::Next() : ItemToPlace;
		u32 *actualItemToShow = useDropChain ? actualItemToPlace : ItemToShow;
		u32 animInstance;
		u8 currentIndex = GameHelper::GetOnlinePlayerIndex();
		bool forced = (currentIndex != GameHelper::GetActualPlayerIndex()) && (GameHelper::GetActualPlayerIndex() <= 3);
		if (GameHelper::GetLockedSpotIndex(worldx, worldy, roomID) != 0xFFFFFFFF) return 0;
		if (pItemAtCoords == nullptr) return 0;
		u32 *actualItemToReplace = ItemToReplace == 0xFFFFFFFF ? pItemAtCoords : &ItemToReplace;
		if (*actualItemToReplace != *pItemAtCoords && !(*actualItemToReplace == 0x7FFE && *pItemAtCoords == 0x80007FFE)) return 0;
		if (Player::GetLocation() == 0xFF && (*actualItemToPlace == 0x207A || *actualItemToPlace == 0x2079)) return 0;
		if (Player::GetLocation() != 0xFF && (*actualItemToPlace <= 0xFF || (*actualItemToPlace & 0x80000000) != 0 || ID == 8 || ID == 9 || ID == 0xE || ID == 0xF)) return 0;
		if (ID == 9 || ID == 0xF) *actualItemToReplace = 0x26;
		if (ID == 8 || ID == 0xE) *actualItemToReplace = 0x3E;
		if (ID == 3 || ID == 4 || ID == 5) *actualItemToReplace = 0x2001;
		if (roomID != 0xA5) {
			*(vu32 *)0x59EC90 = 0xE3A00000 + roomID;
			for (u8 i = 0; i < 4; i++) {
				if (Player::Exists(i) && Player::GetRoom(i) == roomID) {
					Process::Write32(0x59EBF8, 0xE3A00000 + i);
				}
			}
			Sleep(Milliseconds(5));
		}
		if (ID == 0x21) {
			if (player == 0) return 0;
			if (!h.flags.isEnabled) {
				Process::Write32(0x59EDD8, 0xE1A00000);
				Sleep(Milliseconds(5));
				h.Initialize(0x59EDD8, (u32)&pseudoType0x21);
				h.Enable();
			}
			Sleep(Milliseconds(5));
		}
		else {
			h.Disable();
			Process::Write32(0x59EDD8, 0xEB000019);
		}
		GameHelper::PlaceItem(ID == 0x21 ? 0xF : ID, *actualItemToReplace == 0x7FFE ? &crashPreventItem : actualItemToReplace, *actualItemToPlace == 0x7FFE && ID != 0x1C && !(ID >= 0x6 && ID <= 0x9) && ID != 0xE && ID != 0xF ? &crashPreventItem : actualItemToPlace, *actualItemToShow == 0x7FFE && ID != 0x1C && !(ID >= 0x6 && ID <= 0x9) && ID != 0xE && ID != 0xF ? &crashPreventItem : actualItemToShow, worldx, worldy, u0, u1, ID == 0x8 || ID == 0xE ? 1 : 0, u3, u4);
		if (roomID != 0xA5) {
			Sleep(Milliseconds(5));
			Process::Write32(0x59EC90, 0xEBF5624D);
			Process::Write32(0x59EBF8, 0xEBF59CDB);
		}
		if (!(ID >= 0xa && ID <= 0xd) && ID != 0x22) {
			bool noWait = false;
			if (player == 0) return 0;
			animInstance = Player::GetAnimationInstance(player, 0, 0, 0);
			*(u32 *)(player + 0x844) = 0;
			*(u8 *)(player + 0x8CC) = ID;
			if (waitAnim == 0x5D || waitAnim == 0x6B || waitAnim == 0x4F || waitAnim == 0x4C || waitAnim == 0x50 || waitAnim == 0x52 || waitAnim == 0x5A || waitAnim == 0x5F || waitAnim == 0x60 || waitAnim == 0x61 || waitAnim == 0x7E || waitAnim == 0x87 || waitAnim == 0xAC)
				noWait = true;
			if (waitAnim == 0x3D || waitAnim == 0x40 || waitAnim == 0x41) {
				noWait = true;
				goto noWaitPick;
			}
			if (ID >= 0x1 && ID <= 0x6 && !noWait) {
				if (forced) {
					switch (ID) {
					case 1:
					case 2:
						autoWaitAnim = 0x3D;
						break;
					case 3:
					case 4:
					case 5:
						autoWaitAnim = 0x41;
						break;
					case 6:
						autoWaitAnim = 0x40;
						break;
					}
				}
				else {
					autoWaitAnim = 0x3C;
				}
				noWaitPick:
				Process::Write32(animInstance + 0xE, *(u32 *)actualItemToReplace);
				Process::Write8(animInstance + 0x12, ID == 0x21 ? *(u8 *)(player + 0x47C) : worldx);
				Process::Write8(animInstance + 0x13, ID == 0x21 ? *(u8 *)(player + 0x480) : worldy);
				Process::Write8(animInstance + 0x15, ID);
				(CallFuncWrapper( 0x5D3504 ))(animInstance + 0x17, (u32)Player::GetCoordinates(currentIndex));
			}
			else {
				*(u8 *)(animInstance + 0xE) = ID == 0x21 ? *(u8 *)(player + 0x47C) : worldx;
				*(u8 *)(animInstance + 0xF) = ID == 0x21 ? *(u8 *)(player + 0x480) : worldy;
				*(u16 *)(animInstance + 0x10) = *(u16 *)actualItemToReplace == 0x7FFE ? 0x2001 : *(u16 *)actualItemToReplace;
				if (waitAnim == 0x60 || waitAnim == 0x61) {
					volatile float *pCoords = Player::GetCoordinates();
					if (pCoords != nullptr) {
						*pCoords = worldx * 0x20 + 0x10;
						*((float *)((vu32)pCoords + 8)) = worldy * 0x20 + 0x10;
					}
				}
				if (ID == 0x7 && !noWait && waitAnim != 0x5C && waitAnim != 0x6A) autoWaitAnim = forced ? 0x5D : 0x5C;
				else if (ID >= 0x8 && ID <= 0xF && !noWait && waitAnim != 0x6A) autoWaitAnim = forced ? 0x6B : 0x6A;
				else if (forced && !noWait) autoWaitAnim = 0x4C;
				if (((ID >= 0x13 && ID <= 0x1C) || autoWaitAnim == 0x4A) && !noWait && !forced) {
					autoWaitAnim = 0x4A;
					*(u32 *)0x680F28 = 0xE3A00000;
					Process::Write16(animInstance + 0x10, ID);
					Sleep(Milliseconds(5));
				}
				else if (forced) autoWaitAnim = 0x4F;
			}
			if (waitAnim == 0x87) {
				(CallFuncWrapper( 0x5D3504 ))(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy + 1, coords));
				Process::Write8(animInstance + 0x12,0xC0);
				Process::Write8(animInstance + 0x13, 0);
				Process::Write8(animInstance + 0x14, 2);
			}
			else if (waitAnim == 0xAC) {
				(CallFuncWrapper( 0x5D3504 ))(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy + 1, coords));
				Process::Write8(animInstance + 0x12, 0);
				(CallFuncWrapper( 0x5D3504 ))(animInstance + 0x13, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy + 1, coords));
				Process::Write8(animInstance + 0x17, 0);
				Process::Write8(animInstance + 0x18, 2);
			}
			else if (waitAnim == 0x7E) {
				(CallFuncWrapper( 0x5D3504 ))(animInstance + 0xF, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy - 1, coords));
				Process::Write8(animInstance + 0x13, 0);
				(CallFuncWrapper( 0x5D3504 ))(animInstance + 0x17, (u32)GameHelper::WorldCoordsToCoords(worldx, worldy - 1, coords));
				Process::Write8(animInstance + 0x16, 1);
				Process::Write16(animInstance + 0x14, 2);
			}
			if (currentIndex == GameHelper::GetActualPlayerIndex()) Player::ExecuteAnimation(player, autoWaitAnim, animInstance, 0);
			else Player::SendAnimPacket(GameHelper::GetActualPlayerIndex(), animInstance, autoWaitAnim, roomID == 0xA5 ? Player::GetRoom(currentIndex) : roomID, currentIndex);	
			if (!noWait) {
				Sleep(Milliseconds(40));
				*(u32 *)0x680F28 = 0xEBFC52D7;
			}
		}
		return true;
	}
	
	bool GameHelper::RemoveItems(bool trample, u8 wX, u8 wY, u8 width, u8 length, bool allowAbort, bool removeEverything)
	{
		bool res = false;
		//if (Player::GetLocation() == 0xFF)
		//{
		u8 x = wX, y = wY;
		u32 count = 0, player = Player::GetInstance(4, 1);
		if (player == 0) return res;
		
		if (removeEverything)
		{
			if (Player::GetLocation() == 0xFF)
			{
				x = 0x10;
				y = 0x10;
			}
			else
			{
				x = 0;
				y = 0;
			}
		}

		res = true;
		if (trample)
		{
			Process::Write32(USA0x00597FAC, 0xE1A00000);// 0A000039		BEQ		0x598098
			Process::Write32(USA0x00597F38, 0xE1A00000);// 0A000056		BEQ		0x598098
			Process::Write32(USA0x00597F54, 0xE1A00000);// 0A00004F		BEQ		0x598098
		}
		while (res && (x - wX < width || removeEverything))
		{
			while (res && (y - wY < length || removeEverything))
			{
				if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) != 0)
				{
					if (*GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) != 0x7FFE)
					{
						count++;
						if (trample)
						{
							//*Player::GetCoordinates() = x * 0x20 + 0x10;
							//*(float *)((u32)Player::GetCoordinates() + 8) = y * 0x20 + 0x8;
							GameHelper::TrampleAt(x, y);
							//Sleep(Milliseconds(8));
						}
						else
						{
							GameHelper::PlaceItemWrapper(0x6, 0xFFFFFFFF, (u32 *)0x95CFFC, (u32 *)0x95CFFC, x, y, 0, 0, 0, 0, 0, 0x3C, 0xA5);
						}
						Controller::Update();
						if (Controller::IsKeyPressed(Key::B) && allowAbort)
						{
							OSD::Notify("Aborted.");
							goto end;
						}
					}
				}
				else
				{
					res = false;
				}
				y++;
			}
			res = true;
			if (removeEverything)
			{
				if (Player::GetLocation() == 0xFF) y = 0x10;
				else y = 0;
			}
			else y = wY;
			x++;
			if ((u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), x, y, 0) == 0)
			{
				res = false;
			}
		}
	end:
		if (trample)
		{
			Process::Write32(USA0x00597FAC, 0x0A000039);// 667行
			Process::Write32(USA0x00597F38, 0x0A000056);// 668行
			Process::Write32(USA0x00597F54, 0x0A00004F);// 669行
		}
		else Process::Write32(USA0x0059A268, 0x31F2DBF0);// 0095CFFC     よくわからん，95CFFCの中身は7FFEだった
		OSD::Notify(std::to_string(count) << " items removed!");
		return true;
		//}
	}
	
	void GameHelper::Particles(u32 particleID, float *floats, u32 u0, u32 u1)
	{
		if (floats == nullptr) return;
		(CallFuncWrapper( 0x207AD0 ))(particleID, (u32)floats, u0, u1);
	}
	
	void GameHelper::clearLockedSpot(u8 wX, u8 wY, u8 roomID, u32 param_4)
	{
		(CallFuncWrapper( USA0x005A1278 ))(wX, wY, roomID, param_4);
	}
	u32 GameHelper::createLockedSpot(u8 DropID, u8 wX, u8 wY, u8 roomID, bool sendPkt)
	{
		u32 index;
		
		// 5A13C8	E24DD01C	SUB		SP, SP, #28  ; 0x1C
		// 5A13CC	E1A07001	MOV		R7, R1
		if (*(u32 *)USA0x005A13C8 == 0xE3A00000)
		{
			*(u32 *)USA0x005A13C8 = 0xE24DD01C;
			*(u32 *)USA0x005A13CC = 0xE1A07001;
			
			index = (CallFuncWrapper( USA0x005A13C4 ))(DropID, wX, wY, roomID, sendPkt);

			*(u32 *)USA0x005A13C8 = 0xE3A00000;
			*(u32 *)USA0x005A13CC = 0xE8BD83F0;

			return index;
		}
		else
		{
			return (CallFuncWrapper( USA0x005A13C4 ))(DropID, wX, wY, roomID, sendPkt);
		}
	}

	u32 GameHelper::GetLockedSpotIndex(u8 wX, u8 wY, u8 roomID)
	{
		return (CallFuncWrapper( USA0x005A11BC ))(wX, wY, roomID);
	}
	
	void GameHelper::TrampleAt(u8 wX, u8 wY)
	{
		u32 pItem = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), wX, wY, 0);
		if (pItem != 0)
		{
			u8 room = Player::GetRoom(GameHelper::GetActualPlayerIndex()); 
			if (GameHelper::GetOnlinePlayerCount() != 0)
			{	
				TramplePkt trmplPktD{ *(u32 *)pItem, room, wX, wY, 0 };
				(CallFuncWrapper( 0x623F80 ))(0x47, 4, (u32)&trmplPktD, 8);
			}
			(CallFuncWrapper( 0x168E08 ))(wX, wY, 0, room, 0x955FF4);
			(CallFuncWrapper( 0x59DA7C ))(wX, wY, 1);
		}
	}
	
	float *GameHelper::WorldCoordsToCoords(u8 wX, u8 wY, float res[3])
	{
		volatile float *coords = Player::GetCoordinates();
		if (coords != nullptr) res[1] = *(volatile float *)((u32)coords + 4);
		res[0] = (float)(wX * 0x20 + 0x10);
		res[2] = (float)(wY * 0x20 + 0x10);
		return &res[0];
	}
	
	
	
}

