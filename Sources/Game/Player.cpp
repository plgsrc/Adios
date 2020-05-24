#include "CTRPluginFramework.hpp"
#include "CallFuncWrapper.hpp"
#include "strings.hpp"
#include "Address.h"
#include "Game.hpp"

namespace CTRPluginFramework
{
	u32 Player::GetInstance(u8 PlayerIndex, bool noChecks)
	{
		return (CallFuncWrapper(USA0x005C3EA0))(PlayerIndex, noChecks);
	}
	
	u32 Player::TryGetInstance(u8 PlayerIndex)
	{
		return 0;
	}
	
	bool Player::Exists(u8 PlayerIndex)
	{
		if (GameHelper::GetOnlinePlayerCount() == 0 && PlayerIndex == GameHelper::GetOnlinePlayerIndex()) return true;
		//if (*(u8 *)(0x31FE28ED + PlayerIndex) != 0xA5) return true;
		if (*(u8 *)(0x31F5AC54 + PlayerIndex) != 0xA5) return true;
		return false;
	}
	
	bool Player::GetWorldCoords(u32 *worldx, u32 *worldy, u8 PlayerIndex, bool noChecks)
	{
		return (CallFuncWrapper(USA0x005C13AC))((u32)worldx, (u32)worldy, PlayerIndex, noChecks);
	}
	
	u32 Player::GetAnimationInstance(u32 playerInstance, u8 someVal1, u8 someVal2, u32 encVal)
	{
		return (CallFuncWrapper(USA0x006576F8))(playerInstance, someVal1, someVal2, encVal);
	}
	
	bool Player::ExecuteAnimation(u32 pPlayerObj, u8 animID, u32 pAnimObj, bool u0)
	{
		return (CallFuncWrapper(USA0x0064DB90))(pPlayerObj, animID, pAnimObj, u0);
	}
	
	bool Player::ExecuteAnimationWrapper(u8 pIndex, u8 animID, u32 animItem, u8 emotion, u16 snake, u16 sound, bool u0, u8 wX, u8 wY, bool directSend, u8 appearance[])
	{
		u8 actualIndex = *(u8 *)((*(u32 *)0x94D644) + 0x13268);
		bool forced = (pIndex != actualIndex) && (actualIndex <= 3);
		bool needOverwrite = (GameHelper::GetOnlinePlayerIndex() != pIndex);
		u32 player = Player::GetInstance(pIndex, 1);
		u32 animInstance;
		float coords[3];
		
		if (player == 0) return 0;
		animInstance = Player::GetAnimationInstance(player, 0, 0, 0);
		if (animInstance == 0) return 0;
		
		//Process::Write32((u32)&pfunction2, USA0x005D4C88);
		(CallFuncWrapper(USA0x005D4C88))(animInstance + 0x2, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords)); 

		if (animID == 0xB7 || animID == 0x71 || animID == 0x72 || animID == 0x73 || animID == 0x24 || animID == 0x3E || animID == 0x3F || animID == 0x3D || animID == 0x38 || animID == 0x92 || animID == 0x93)
		{
			Sleep(Milliseconds(75));
			Process::Write16(animInstance + 0xE, animItem & 0xFFFF);
			Process::Write16(animInstance + 0x10, (animItem >> 0x10) & 0xFFFF);
			if (animID == 0x3E)
			{
				Process::Write16(animInstance + 0x12, snake);
				Process::Write32(animInstance + 0x14, 0x00020101);
			}
			if (animID == 0x38)
			{
				Process::Write8(animInstance + 0x12, 0x01);
			}
		}
		else if (animID == 0xAF || animID == 0x88)
		{
			Process::Write8(animInstance + 0xE, emotion & 0xFF);
			Process::Write8(animInstance + 0xF, 1); // sets animation to 0x6 after executing
			Process::Write8(animInstance + 0x15, 4); // continuous emote
		}
		else if (animID == 0xC4)
		{
			Process::Write16(animInstance + 0xE, snake & 0xFFF);
			Process::Write16(animInstance + 0x10, sound & 0xFFFF);
			Process::Write8(animInstance + 0x13, 1);
			Process::Write16(animInstance + 0x15, 0xFFFE);
			Process::Write8(animInstance + 0x17, 4);
			Process::Write8(animInstance + 0x18, *(u8 *)(player + 0x2F));
		}
		else if (animID == 0xC5)
		{
			Process::Write16(animInstance + 0xE, snake & 0xFFF);
			Process::Write16(animInstance + 0x10, snake & 0xFFFF);
			Process::Write8(animInstance + 0x12, *(u8 *)(player + 0x2F));
			Process::Write8(animInstance + 0x13, 1);
			Process::Write8(animInstance + 0x17, 5);
		}
		else if (animID == 0x45)
		{
			Process::Write32(USA0x0065F7CC, 0xE1A00000);
			
			Process::Write16(animInstance + 0xE, sound & 0xFFFF);
			Process::Write8(animInstance + 0x11, 1);
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0x12, (u32)Player::GetCoordinates(pIndex));
		}
		else if (animID == 0x3C)
		{
			Process::Write32(animInstance + 0xE, animItem);
			Process::Write8(animInstance + 0x12, wX);
			Process::Write8(animInstance + 0x13, wY);
			Process::Write8(animInstance + 0x15, emotion);
		}
		else if (animID == 0x4A)
		{
			Process::Write8(animInstance + 0xE, wX);
			Process::Write8(animInstance + 0xF, wY);
			Process::Write16(animInstance + 0x10, emotion);
		}
		else if (animID == 0x36)
		{
			Process::Write32(animInstance + 0xE, animItem);
			Process::Write8(animInstance + 0x12, emotion);
			Process::Write32(animInstance + 0x1C, 0x01480000);
		}
		else if (animID == 0x33)
		{
			//Process::Write32((u32)&pfunction2, USA0x005D4C88);
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0x10, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
		}
		else if (animID >= 0x7B && animID <= 0x7F)
		{
			//Process::Write32((u32)&pfunction2, USA0x005D4C88);
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0xF, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			Process::Write8(animInstance + 0x13, *(u8 *)(player + 0x2F));
		}
		else if (animID == 0x1C || animID == 0x25 || animID == 0x1D || animID == 0xC1 || animID == 0xE2 || animID == 0xE5 || (animID >= 0xA8 && animID <= 0xAE) || (animID >= 0x80 && animID <= 0x87) || (animID >= 0x2B && animID <= 0x33))
		{
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0x2, (u32)Player::GetCoordinates(pIndex));
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			Process::Write8(animInstance + 0x12, *(u8 *)(player + 0x2F));
			Process::Write8(animInstance + 0x13, emotion);
		}
		else if (animID == 0xB5 || animID == 0xB3 || animID == 0xB4)
		{
			(CallFuncWrapper(USA0x005D4C88))(animInstance + 0xE, (u32)GameHelper::WorldCoordsToCoords(wX, wY, coords));
			Process::Write32(animInstance + 0x12, animItem);
		}
		else if (animID == 0xB9)
		{
			Process::Write8(animInstance + 0xC, 0x10);
			Process::Write8(animInstance + 0xD, 0);
			Process::Write8(animInstance + 0xE, appearance[0]);
			Process::Write8(animInstance + 0xF, appearance[1]);
			Process::Write8(animInstance + 0x10, appearance[2]);
			Process::Write16(animInstance + 0x11, animItem & 0xFFFF);
		}
		else
		{
			Process::Write8(animInstance + 0xE, wX);
			Process::Write8(animInstance + 0xF, wY);
			Process::Write16(animInstance + 0x10, animItem);
			//Process::Write8(animInstance + 0x13, 2);
		}
		
		if (!directSend)
		{
			if (forced && needOverwrite)
			{
				Process::Write32(USA0x005C3EB0, 0xE3A00000 + pIndex);// 2BF5080E		BLCS	0x305EF0
				Process::Write32(USA0x001ABADC, 0xE3A00000 + pIndex);// E3A00004		MOV		R0, #4  ; 0x4
			}
			if (forced)
			{
				Process::Write32(USA0x002FEB64, 0xE1A00000);// 1A000002		BNE		0x2FEB74	(E3540004)
				Process::Write32(USA0x00677530, 0xE1A01006);// 01A01006		MOVEQ	R1, R6
				Process::Write32(USA0x00677534, 0xEBFD31D0);// 0BFD31D0		BLEQ	0x5C3C7C	(E92D4070)
				Process::Write32(USA0x00677454, 0xE1A00000);// 0A000038		BEQ		0x67753C	(E1A00002)
				Process::Write32(USA0x006774F0, 0xE1A00000);// 0A00001C		BEQ		0x677568	(E5D401A9)
				Process::Write32(USA0x00677504, 0xE1A00000);// 1A000017		BNE		0x677568
				
				// E5D12268		LDRB	R1, [R1, #616]
				Process::Write32(USA0x00628B54, 0xE3A01000 + pIndex); // in sendPkt Function GetOnlinePlayerIndex inline
				Sleep(Milliseconds(5));
			}
			Player::ExecuteAnimation(player, animID, animInstance, u0);
			if (forced)
			{
				Sleep(Milliseconds(5));
				Process::Write32(USA0x00628B54, 0xE5D11268);// この辺は上と同じ
				Process::Write32(USA0x00677530, 0x01A01006);
				Process::Write32(USA0x00677534, 0x0BFD31D0);
				Process::Write32(USA0x00677504, 0x1A000017);
				Process::Write32(USA0x006774F0, 0x0A00001C);
				Process::Write32(USA0x00677454, 0x0A000038);
				Process::Write32(USA0x005C3EB0, 0x2BF5080E);
				Process::Write32(USA0x002FEB64, 0x1A000002);
				Process::Write32(USA0x001ABADC, 0xE3A00004);
			}
		}
		else
		{
			if (forced) Player::SendAnimPacket(actualIndex, animInstance, animID, Player::GetRoom(pIndex), pIndex);
			else Player::ExecuteAnimation(player, animID, animInstance, u0);
		}
		Sleep(Milliseconds(25));
		return 1;
	}
	
	bool Player::Pickup(u32 PlayerObj, u8 worldx, u8 worldy, bool u0, bool u1, bool u2)
	{
		return (CallFuncWrapper(USA0x00661524))(PlayerObj, worldx, worldy, u0, u1, u2);
	}
	
	float *Player::GetCoordinates(u8 PlayerIndex)
	{
		u32 i = GetInstance(PlayerIndex, 1);
		if (i != 0) i += 0x14;
		return (float *)i;
	}
	
	bool Player::GetCoordinatesRef(u8 PlayerIndex, float &X, float &Z)
	{
		u32 i = GetInstance(PlayerIndex, 1);
		if (i != 0) 
		{
			i += 0x14;
			X = *(volatile float *)i;
			Z = *(volatile float *)(i + 8);
			return true;
		}
		else return false;
	}
	
	float *Player::GetCollisionSize(u8 PlayerIndex)
	{
		u32 i = GetInstance(PlayerIndex, 1);
		if (i != 0) i += 0x4F4;
		return (float *)i;
	}
	
	u8 *Player::GetAnimation(u8 PlayerIndex)
	{
		u8 *ptr = (u8 *)(GetInstance(PlayerIndex, 1) + 0x1A9);
		if ((u32)ptr != 0x1A9) return ptr;
		else return 0;
	}
	
	u8 Player::GetLocation()
	{
		return *(u8 *)0x33099F7C;
	}
	
	u32 *Player::GetHeldItem()
	{
		return nullptr;
	}
	
	float *Player::GetVelocity(u8 PlayerIndex)
	{
		return (float *)(GetInstance(PlayerIndex, 1) + 0x40);
	}
	
	u8 Player::GetRoom(u8 PlayerIndex)
	{
		u32 animObjPtr;
		animObjPtr = (CallFuncWrapper(USA0x005C3DDC))(PlayerIndex);
		return animObjPtr == 0 ? 0xFF : *(u8 *)animObjPtr;
	}
	
	u32 * Player::GetInvSlotAddr(int slot)
	{
		return (u32*)( *(u32*)0xAA914C + 0x6BD0 + slot * 0x4 );
	}
	
	void Player::SendAnimPacket(u8 senderIndex, u32 animObj, u8 animID, u8 roomID, u8 targetPlayerIndex)
	{
		Process::Write8(animObj, roomID);
		Process::Write8(animObj + 1, animID);
		Process::Write32(USA0x00628B54, 0xE3A01000 + targetPlayerIndex);
		Process::Write8(USA0x005C3CAC, targetPlayerIndex);
		Sleep(Milliseconds(5));
		(CallFuncWrapper(USA0x005C3C7C))(targetPlayerIndex, animObj);
		Sleep(Milliseconds(5));
		Process::Write32(USA0x00628B54, 0xE5D11268);
		Process::Write8(USA0x005C3CAC, 4);
	}
	
}

