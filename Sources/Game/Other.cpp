#include "CTRPluginFramework.hpp"
#include "strings.hpp"
#include "Address.h"
#include "Game.hpp"

namespace CTRPluginFramework
{
	std::vector<u32> items;
	std::vector<std::string> keyBoardItems;
	u8 index;
	bool useDropChain = false;
	Keyboard *optDCKB;
	Keyboard *DCKB;
	u32 *defPtr;
	u32 defItem;

	volatile u32(*pfunction0)();
	volatile u32(*pfunction1)(u32 param1);
	volatile u32(*pfunction2)(u32 param1, u32 param2);
	volatile u32(*pfunction3)(u32 param1, u32 param2, u32 param3);
	volatile u32(*pfunction4)(u32 param1, u32 param2, u32 param3, u32 param4);
	volatile u32(*pfunction5)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5);
	volatile u32(*pfunction6)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6);
	volatile u32(*pfunction7)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7);
	volatile u32(*pfunction8)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8);
	volatile u32(*pfunction9)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9);
	volatile u32(*pfunction10)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9, u32 param10);
	volatile u32(*pfunction11)(u32 param1, u32 param2, u32 param3, u32 param4, u32 param5, u32 param6, u32 param7, u32 param8, u32 param9, u32 param10, u32 param11);
	
	bool NPC::GetCoordinatesRef(u32 npc, float &X, float &Z)
	{
		if (npc == 0) return false;
		if (*(u32 *)(npc + 0x14) == 0) return false;
		X = *(volatile float *)(npc + 0x14);
		Z = *(volatile float *)(npc + 0x1C);
		return true;
	}

	std::string IDList::GetItemName(u32 ID)
	{
		/*
		std::string t;
		if ((ID & 0xFFFF) > 0x2000 && (ID & 0xFFFF) < 0x372B)
		{
			u32 addr = 0x31758960;
			addr = addr + *(u32 *)(addr + 4 + 4 * (ID - 0x2000)) + 0xC;
			u8 i = 0;
			while (*(u8 *)(i * 2 + addr) != 0)
			{
				t.push_back(*(char *)(i * 2 + addr));
				i++;
			}
		}
		else t.assign("???");
		return t;
		*/
		return "???";
	}

	std::string IDList::GetBuildingName(u8 ID)
	{
		Process::Write32((u32)&pfunction1, USA0x005CA20C);// E35000FC	CMP		R0, #252  ; 0xFC
		return (char *)pfunction1(ID);
	}

	std::string IDList::GetSnakeName(u16 ID)
	{
		/*
		if (ID < 0x276 && ID != 0)
			return Color::Green << (std::string)((char *)(*(u32 *)(ID * 0x10 + 0x866D08)));
		else return Color::Red << "Invalid ID!";
		*/
		return "???";
	}
	
	std::string IDList::GetRoomName(u8 ID)
	{
		Process::Write32((u32)&pfunction1, USA0x005B4BE4);// E35000A5	CMP		R0, #165  ; 0xA5
		if (ID <= 0xA5) return Color::Green << (std::string)((char *)(pfunction1(ID)));
		else return Color::Red << "Invalid ID!";
	}

	u8 Item::GetBehaviour(u32 *pItem)
	{
		Process::Write32((u32)&pfunction1, USA0x002FCCD0);// E92D4010	STMFD	SP!, {R4,LR}
		pfunction1((u32)pItem);
		return 0;
	}
	
	void Item::hasTreeBehaviour(u32 *pItem)
	{
		Process::Write32((u32)&pfunction1, USA0x002FE930);// E92D4010	STMFD	SP!, {R4,LR}
		pfunction1((u32)pItem);
	}
	
	
	u32 Camera::GetInstance()
	{
		return *(u32 *)0x94a880;
	}
	void Camera::AddToX(float val)
	{
		*(float *)(Camera::GetInstance() + 4) += val;
	}
	void Camera::AddToY(float val)
	{
		*(float *)(Camera::GetInstance() + 8) += val;
	}
	void Camera::AddToZ(float val)
	{
		*(float *)(Camera::GetInstance() + 0xC) += val;
	}
	void Camera::AddToYRotation(u16 val)
	{
		*(u16 *)(Camera::GetInstance() + 0x1C) += val;
	}

	void ItemSequence::init(u32 defaultPtr)
	{
		optDCKB = new Keyboard("Choose option:");
		DCKB = new Keyboard("Enter new ID:\n(B to remove)");
		items.clear();
		items.push_back(defaultPtr);
		defPtr = (u32 *)defaultPtr;
		optDCKB->CanAbort(true);
		DCKB->CanAbort(true);
		DCKB->IsHexadecimal(true);
		DCKB->SetMaxLength(8);
		DCKB->DisplayTopScreen = true;
	}
	u32 *ItemSequence::Next()
	{
		if (items.size() - 1 > index) index++;
		else index = 0;
		if (index == 0) return defPtr;
		else return &items.at(index);
	}
	u32 ItemSequence::PeekNext()
	{
		if (index + 1 == items.size()) return *defPtr;
		else return items.at(index + 1);
	}
	bool ItemSequence::openKB()
	{
		int8_t val;
		u32 newItem;
		keyBoardItems.clear();
		keyBoardItems.push_back((items.size() == 1 || index + 1 == items.size() ? "->Item in first slot" : "Item in first slot"));
		for (u8 i = 1; i < items.size(); i++) keyBoardItems.push_back((index + 1 == i  ? "->" : "") << Hex(items.at(i)));
		keyBoardItems.push_back("Add...");
		optDCKB->Populate(keyBoardItems);
		val = optDCKB->Open();
		switch (val)
		{
		case -1:
			return false;

		case 0:
			Sleep(Milliseconds(100));
			newItem = *defPtr;
			if (DCKB->Open(newItem, newItem) == 0) *defPtr = newItem;
			break;

		default:
			Sleep(Milliseconds(100));
			if (val + 1 == keyBoardItems.size())
			{
				if (DCKB->Open(newItem) == 0) items.push_back(newItem);
			}
			else
			{
				if (DCKB->Open(items.at(val), items.at(val)) == -1)	items.erase(items.begin() + val);
			}
			break;
		}
		Sleep(Milliseconds(100));
		openKB();
		return true;
	}
	void ItemSequence::Switch(bool enable)
	{
		useDropChain = enable;
		//if (enable) defItem = *(u32 *)0x31F2DBF0;
		//if (!enable) *(u32 *)0x31F2DBF0 = defItem;
	}
	bool ItemSequence::Enabled()
	{
		return useDropChain;
	}
	
	
	
}

