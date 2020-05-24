#include "Cheats.hpp"

namespace CTRPluginFramework
{
	extern bool selecting;
	
	// ------------------------------------------------------
	// 座標移動
	// ------------------------------------------------------
	void CoordinateModifier(MenuEntry *entry)
	{
		volatile float *x = (float*)0x33099e50 + get12c();
		volatile float *z = (float*)0x33099e58 + get12c();
		
		if( Controller::IsKeyDown(A) && !selecting )
		{
			if ( Controller::IsKeyDown(DPadUp   ) ) *z -= *GetArg<float>(entry);
			if ( Controller::IsKeyDown(DPadDown ) ) *z += *GetArg<float>(entry);
			if ( Controller::IsKeyDown(DPadLeft ) ) *x -= *GetArg<float>(entry);
			if ( Controller::IsKeyDown(DPadRight) ) *x += *GetArg<float>(entry);
		}
	}
	
	// ------------------------------------------------------
	// 座標移動・スピード変更
	// ------------------------------------------------------
	void ChangeCoordSpeed(MenuEntry *entry)
	{
		float *s = GetArg<float>(entry);
		Keyboard k("スピード変更");
		k.Open( *s, *s );
	}
	
	// ------------------------------------------------------
	// 壁抜け
	// ------------------------------------------------------
	void WalkOverObjects(MenuEntry* e)
	{
		if (Controller::IsKeysPressed( L + DPadUp ))
		{
			if ( *(u32*)0x64EEF4 == 0xEA000094 )
			{
				OSD::Notify("Walk Over Objects: OFF");
				Process::Write32(0x0064EEF4, 0x0A000094);
				Process::Write32(0x0064EF0C, 0x0A000052);
				Process::Write32(0x0064F070, 0x0A000001);
				Process::Write32(0x0064F0E8, 0xDA000014);
				Process::Write32(0x0064F19C, 0xED841A05);
				Process::Write32(0x0064F1B4, 0xED840A07);
				Process::Write32(0x0064F1B8, 0x0A000026);
				Process::Write32(0x0064F1E4, 0x0A000065);
			}
			else
			{
				OSD::Notify("Walk Over Objects: ON");
				Process::Write32(0x0064EEF4, 0xEA000094);
				Process::Write32(0x0064EF0C, 0xEA000052);
				Process::Write32(0x0064F070, 0xEA000001);
				Process::Write32(0x0064F0E8, 0xEA000014);
				Process::Write32(0x0064F19C, 0xE1A00000);
				Process::Write32(0x0064F1B4, 0xE1A00000);
				Process::Write32(0x0064F1B8, 0xEA000026);
				Process::Write32(0x0064F1E4, 0xEA000065);
			}
		}
	}
	
	// ------------------------------------------------------
	// タッチワープ
	// ------------------------------------------------------
	void TouchWarping(MenuEntry* e)
	{
		u16 baseValueCheck = *(u16*)0x95133a;
		static Coordinates* const PlayerCoordinates = reinterpret_cast<Coordinates* const>(0x33099E50);
		
		touchPosition tp;
		hidTouchRead(&tp);
		
		if (!Controller::IsKeyDown(Touchpad)) return;
		
		int TouchX = tp.px - 70;
		int TouchY = tp.py - 54;
		
		// タッチ範囲制限
		if (TouchX < 0) TouchX = 0;
		if (TouchY < 0) TouchY = 0;
		if (TouchX > 180) TouchX = 180;
		if (TouchY > 152) TouchX = 152;
		
		// オフセット
		float basePoint = 528.0f;
		
		PlayerCoordinates->x = basePoint + (TouchX * 13.7);
		PlayerCoordinates->z = basePoint + (TouchY * 14.8);
	}
	
	// ------------------------------------------------------
	// アニメーション実行
	// ------------------------------------------------------
	void ExecAnimation(MenuEntry *entry)
	{
		Keyboard k;
		static u8 AnimID = 0x39;
		
		if( Controller::IsKeyPressed(X) )
			k.Open(AnimID);
		
		if( Controller::IsKeysPressed(A + DPadUp) )
		{
			u32 ins = Player::GetInstance(4, 1);
			Player::ExecuteAnimation( ins, AnimID, Player::GetAnimationInstance(ins, 0, 0, 0), 0 );
		}
	}
	
	// ------------------------------------------------------
	// 他の人を自分の場所に集める
	// ------------------------------------------------------
	Clock PlayerTeleporter_safeCount;
	bool Teleported = false;
	bool PlayerTeleporter_OSDcb(const Screen &scr)
	{
		const float WaitSc = 15;
		if( !scr.IsTop ) return false;
		
		const std::string msg = Utils::Format("荒らし対策: 次の使用まであと%d秒", (int)WaitSc - (int)( PlayerTeleporter_safeCount.GetElapsedTime().AsSeconds() ));
		scr.DrawRect(0, 0, OSD::GetTextWidth(true, msg), 16, Color::Black);
		scr.DrawSysfont(msg, 0, 0);
		
		if( PlayerTeleporter_safeCount.HasTimePassed(Seconds(WaitSc)) )
			OSD::Stop(PlayerTeleporter_OSDcb);
		
		return true;
	}
	void PlayerTeleporter(MenuEntry *entry)
	{
		if (Controller::IsKeysPressed(L + DPadRight))
		{
			if( !Teleported )
			{
				u32 x, y;
				if (Player::GetWorldCoords(&x, &y, 4, 1))
				{
					for (u8 i = 0; i < 4; i++)
					{
						//Player::ExecuteAnimationWrapper(i, 0x34, 1, 1, 1, 1, 0, x, y, true);
					}
					UnicodeNotify("Teleported players to you.");
					PlayerTeleporter_safeCount.Restart();
					OSD::Run(PlayerTeleporter_OSDcb);
				}
			}
			else
			{
				PlayerTeleporter_safeCount.Restart();
			}
		}
	}
	
	
	
	
	
	
}









