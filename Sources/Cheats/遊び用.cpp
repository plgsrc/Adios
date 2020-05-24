#include "Cheats.hpp"

namespace CTRPluginFramework
{
	u32 emotion = 1, soundID = 0x01000660;
	u16 snake = 1;
	u8 appearance[3] = {0, 0, 0};
	u32 particleID = 0x0214;
	u8 playerindex = 0;
	u32 npc;
	u8 npcEmote;
	u32 npcSnake;
	u8 anim1 = 0x06, roomID = 0, waitAnim = 0x56, animToForce = 6, npcAnim = 0;
	u8 DropType = 0xA, DropRoom = 0xA5;
	u32 animItem = 0x2001, npcAnimItem = 0x2001;
	u32 itemIDToReplace = 0x7FFE;
	std::vector<u16> clipboard;
	bool selecting;
	
	// ドロップ系チートで使用するアイテムID
	u16		UseItem = 0x3339;
	
	// ------------------------------------------------------
	// アイテム変更
	// ------------------------------------------------------
	bool ChangeUseItem_DrawOSD = true;
	bool ChangeUseItem_OSDcb(const Screen &s)
	{
		if( s.IsTop ) s.Draw(Utils::Format("ItemID: %04X", UseItem), 2, 228);
		return true;
	}
	void ChangeUseItem(MenuEntry *entry)
	{
		if( Controller::IsKeysDown(Y + DPadRight) )
		{
			Keyboard k("アイテム変更");
			k.Open(UseItem, UseItem);
		}
	}
	void ChangeUseItem_Option(MenuEntry *entry)
	{
		Keyboard k("アイテムIDの表示", {"オン", "オフ"});
		switch( k.Open() )
		{
			case 0:
			{
				ChangeUseItem_DrawOSD = true;
				OSD::Run(ChangeUseItem_OSDcb);
				break;
			}
			case 1:
			{
				ChangeUseItem_DrawOSD = false;
				OSD::Stop(ChangeUseItem_OSDcb);
				break;
			}
		}
	}
	
	// ------------------------------------------------------
	// アイテムランダマイザ
	// ------------------------------------------------------
	void ItemRandomizer(MenuEntry *entry)
	{
		static u16 min, max;
		
		if( Controller::IsKeyDown(L) )
			UseItem = Utils::Random( min, max );
		
		if( Controller::IsKeysPressed(R + X) )
		{
			Keyboard kb1("開始アイテム"), kb2("終了アイテム");
			kb1.Open(min);
			Sleep(Milliseconds(100));
			kb2.Open(max);
		}
	}
	
	// ------------------------------------------------------
	// マップエディタ
	// ------------------------------------------------------
	u32 selectedX, selectedY, size;
	bool removal = false;
	
	bool MapEditor_OSDcb(const Screen &scr)
	{
		if (scr.IsTop) 
		{
			if( removal ) scr.Draw("Removal mode", 8, 200, Color::ForestGreen);
			scr.Draw(Utils::Format("Size  : %d", size), 8, 210, Color::White);
		}
		return true;
	}
	void MapEditor(MenuEntry *entry)
	{
		if (Player::GetInstance(4, 1) == 0) return;
		if (!selecting) Player::GetWorldCoords(&selectedX, &selectedY, 4, 1);
		u32 pItem;
	    static Clock clock;
		pItem = (u32)GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), selectedX, selectedY, 0);
		u32 player = Player::GetInstance(4, 1);
    	float particleCoords[3]{ 0, *(float *)((u32)Player::GetCoordinates(4) + 4), 0 };
		
		if (player == 0) return;
		
		if (entry->WasJustActivated())
		{ 
			Process::Patch(0x1A08C8, 0xE1A00000);
			Process::Patch(0x1A08CC, 0xE3A00000);
			Process::Patch(0x1A08D0, 0xEB0E011D);
		}
		
		if (Controller::IsKeysPressed(Start + DPadUp))
		{
			if (selecting)
			{
				OSD::Stop(MapEditor_OSDcb);
				Process::Patch(0x1A5128, 0xE2805C01);
				OSD::Notify("Map Editor: OFF",  Color::Red);
				selecting = false;
			}
			else
			{
				OSD::Run(MapEditor_OSDcb);		
				Process::Patch(0x1A5128, 0xE8BD81F0);
				OSD::Notify("Map Editor: ON", Color::Green);
				selecting = true;

				*(float *)(Camera::GetInstance() + 4) = (float)(selectedX * 0x20 + 0x10);
				Camera::AddToY(90.0f);
				*(float *)(Camera::GetInstance() + 0xC) = (float)(selectedY * 0x20 + 0x70);
				Camera::AddToYRotation(0x700);
			}
		}
		
		if (selecting)
		{
			if( clock.HasTimePassed(Milliseconds(100)) || Controller::IsKeyPressed(DPad) )
			{
				u32 k = Controller::GetKeysDown();
				if( k & DPadRight )
				{
					selectedX++;
					Camera::AddToX(32);
				}
				if( k & DPadLeft )
				{
					selectedX--;
					Camera::AddToX(-32);
				}
				if( k & DPadDown )
				{
					selectedY++;
					Camera::AddToZ(32);
				}
				if( k & DPadUp )
				{
					selectedY--;
					Camera::AddToZ(-32);
				}
				clock.Restart();
			}
			
			if( Controller::IsKeysPressed(Start + DPadDown) )
			{
				size++;
				if (size >= 4) size = 0;
			}
			
			if (Controller::IsKeysPressed(Start + DPadLeft))
			{
				if (removal)
					removal = false;
				else
					removal = true;
			}
			
			if( Controller::IsKeyDown(A) )
			{
				if (pItem == 0) return;
				for (int8_t i = -size; i <= size; i++)
				{
					for (int8_t j = -size; j <= size; j++)
					{
						if (!removal)
							GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, (u32*)&UseItem, (u32*)&UseItem, (selectedX + j), (selectedY + i), 0, 0, 0, 0, 0, waitAnim, 0xA5);
						else
							GameHelper::TrampleAt((selectedX + j), (selectedY + i));
						Sleep(Milliseconds(5));
					}
				}
			}
			
			
			for (int8_t i = -size; i <= size; i++) {
				for (int8_t j = -size; j <= size; j++) {
					particleCoords[0] = (selectedX + j) * 0x20 + 0x10;
					particleCoords[2] = (selectedY + i) * 0x20 + 0x10;
					//GameHelper::Particles(particleID, particleCoords, 0x96FC06, 0xADF870);
				}
			}
			
			
		}
		
		if (!entry->IsActivated())
		{			
			OSD::Stop(MapEditor_OSDcb);
			Process::Patch(0x1A5128, 0xE2805C01);
			Process::Patch(0x1A08C8, 0xE3A01040);
            Process::Patch(0x1A08CC, 0xE5900000);
            Process::Patch(0x1A08D0, 0xEB14CAC6);
		}
	}
	
	// ------------------------------------------------------
	// 自動ドロップ
	// ------------------------------------------------------
	u32 AutoDrop_SleepMillisc = 5;
	void AutoDrop(MenuEntry *entry)
	{
		static bool enabled = false;
		u8 wX, wY;
		u8 size = *GetArg<u8>(entry, 3);
		static Clock clock;
		
		if (Controller::IsKeysPressed(Y + DPadLeft) && !selecting )
		{
			if (enabled)
			{
				enabled = false;
				OSD::Notify("AutoDrop: Disabled", Color::Red);
			}
			else
			{
				enabled = true;
				OSD::Notify("AutoDrop: Enabled", Color::ForestGreen);
			}
		}
		
		if( !enabled ) return;
		if( clock.HasTimePassed(Milliseconds((int)AutoDrop_SleepMillisc)) && Player::GetWorldCoords((u32*)&wX, (u32*)&wY , 4, 1) )
		{
			clock.Restart();
			u32 sx = wX - size / 2;
			u32 sy = wY - size / 2;
			for( u8 x = 0; x < size; x++ )
				for( u8 y = 0; y < size; y++ )
					if( * GameHelper::GetItemAtWorldCoords(GameHelper::GetCurrentMap(), sx + x, sy + y, 0) == 0x7FFE )
					{
						GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, (u32 *)&UseItem, (u32 *)&UseItem, sx + x, sy + y, 0, 0, 0, 0, 0, waitAnim, 0xA5);
						return;
					}
		}
	}
	void AutoDrop_Option(MenuEntry *entry)
	{
		Keyboard k1("", {"サイズ変更", "スリープ時間変更"});
		
		const int r = k1.Open();
		OSD::SwapBuffers();
		
		switch( r )
		{
			case 0:
			{
				u8 *size = GetArg<u8>(entry, 3);
				Keyboard k2("サイズ変更");
				k2.IsHexadecimal(false);
				k2.Open( *size, *size );
				break;
			}
			case 1:
			{
				Keyboard k2("スリープ時間変更\n単位: ミリセカンド( 1000 = 1秒 )");
				k2.IsHexadecimal(false);
				k2.Open(AutoDrop_SleepMillisc);
				break;
			}
		}
	}
	
	// ------------------------------------------------------
	// タッチドロップ
	// ------------------------------------------------------
	bool TouchDrop_Enabled = true;
	void TouchDrop(MenuEntry *entry)
	{
		if( !TouchDrop_Enabled ) return;
		
		if( Controller::IsKeyDown(Touchpad) && !selecting && *(u8*)0x949C30 == 1 )
		{
			UIntVector pos = Touch::GetPosition();
			u32 worldx, worldy; 
			switch (*(u8*)0x95133A)
			{
			case 0:// 村
				if (pos.x >= 68 && pos.y >= 50 && pos.x <= 245 && pos.y <= 192)
				{
					worldx = ((pos.x - 68) / 2.2125) + 0x10;
					worldy = ((pos.y - 50) / 2.2125) + 0x10;
				}
				else return;
				break;
			case 0x68:// 島
				if (pos.x >= 116 && pos.y >= 70 && pos.x <= 201 && pos.y <= 157)
				{
					worldx = ((pos.x - 116) / 2.65) + 0x10;
					worldy = ((pos.y - 70) / 2.65) + 0x10;
				}
				else return;
				break;
			default:
				return;
			}
			u32 player = Player::GetInstance(4, 1);
			if (player != 0) 
			{
				GameHelper::PlaceItemWrapper(DropType, itemIDToReplace, (u32*)&UseItem, (u32*)&UseItem, worldx, worldy, 0, 0, 0, 0, 0, waitAnim, 0xA5);
			}
		}
	}
	
	// ------------------------------------------------------
	// タッチアイテム削除
	// ------------------------------------------------------
	void TouchTrample(MenuEntry *entry)
	{
		if( Controller::IsKeyDown(Touchpad) && !selecting && *(u8*)0x949C30 == 1 )
		{
			UIntVector pos = Touch::GetPosition();
			u32 worldx, worldy; 
			switch (*(u8*)0x95133A)
			{
			case 0:// 村
				if (pos.x >= 68 && pos.y >= 50 && pos.x <= 245 && pos.y <= 192)
				{
					worldx = ((pos.x - 68) / 2.2125) + 0x10;
					worldy = ((pos.y - 50) / 2.2125) + 0x10;
				}
				else return;
				break;
			case 0x68:// 島
				if (pos.x >= 116 && pos.y >= 70 && pos.x <= 201 && pos.y <= 157)
				{
					worldx = ((pos.x - 116) / 2.65) + 0x10;
					worldy = ((pos.y - 70) / 2.65) + 0x10;
				}
				else return;
				break;
			default:
				return;
			}
			
			if (Player::GetInstance(4, 1) != 0) 
				GameHelper::TrampleAt( worldx, worldy );
		}
	}
	
	// ------------------------------------------------------
	// エフェクト
	// ------------------------------------------------------
	void ShowParticles(MenuEntry *entry)
	{
		// ID
		static u16 ParticleID = 0x20c;
		
		// 開始地点(プレイヤーの足元から相対指定)
		float StartCoords[3] =
			{
				-32 * 1.5,
				0,
				-32 * 1.5
			};
		
		// 表示する個数
		u32 Counts[3] =
			{
				4,
				4,
				4
			};
		
		// 表示間隔
		float ParticleSpace = 32.0f;
		
		float ParticleCoords[3];
		
		if ( Controller::IsKeysDown(A + DPadRight) )
		{
			Keyboard k("パーティクルID");
			k.Open(ParticleID);
		}
		
		if ( Controller::IsKeysDown(A + DPadUp) )
		{
			int x, y, z;
			for (x = 0; x < Counts[0]; x++)
				for (y = 0; y < Counts[1]; y++)
					for (z = 0; z < Counts[2]; z++)
					{
						ParticleCoords[0] = Player::GetCoordinates()[0] + StartCoords[0] + x * ParticleSpace;
						ParticleCoords[1] = Player::GetCoordinates()[1] + StartCoords[1] + y * ParticleSpace;
						ParticleCoords[2] = Player::GetCoordinates()[2] + StartCoords[2] + z * ParticleSpace;
						GameHelper::Particles(ParticleID, ParticleCoords);
					}
		}
	}
	
	// ------------------------------------------------------
	// タッチパーティクル
	// ------------------------------------------------------
	void TouchParticles(MenuEntry *entry)
	{
		// ID
		static u16 ParticleID = 0x20c;
		
		if ( Controller::IsKeysDown(A + DPadRight) )
		{
			Keyboard k("パーティクルID");
			k.Open(ParticleID);
		}
		
		if ( Touch::IsDown() )
		{
			u32 x = Touch::GetPosition().x;
			u32 y = Touch::GetPosition().y;
			u8 worldx, worldy;
			
			switch( *(u8*)0x95133A )
			{
				case 0:// 村
				{
					if (x >= 68 && y >= 50 && x <= 245 && y <= 192)
					{
						worldx = (x - 68) / 2.2125;
						worldy = (y - 50) / 2.2125;
						float coord[3] = {(float)(528 + worldx * 0x20), Player::GetCoordinates()[1], (float)(528 + worldy * 0x20)};
						GameHelper::Particles(ParticleID, coord);
					}
				}
				case 0x68:// 島
				{
					if (x >= 116 && y >= 70 && x <= 201 && y <= 157)
					{
						worldx = (x - 116) / 2.65;
						worldy = (y - 70) / 2.65;
						float coord[3] = {(float)(528 + worldx * 0x20), Player::GetCoordinates()[1], (float)(528 + worldy * 0x20)};
						GameHelper::Particles(ParticleID, coord);
					}
				}
			}
		}
	}
	
}


