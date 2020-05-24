#include "Cheats.hpp"
#include "OSDManager.hpp"
#include "CallFuncWrapper.hpp"

namespace CTRPluginFramework
{
	// ------------------------------------------------------
	// セーブメニュー非表示
	// ------------------------------------------------------
	void DisableSaveMenu(MenuEntry *entry)
	{
		Keyboard k("", {"オン", "オフ"});
		
		switch (k.Open())
		{
		case 0:
			Process::Patch(0x1A08C8, 0xE1A00000);
			Process::Patch(0x1A08CC, 0xE3A00000);
			Process::Patch(0x1A08D0, 0xEB0E011D);
			break;
		case 1:
			Process::Patch(0x1A08C8, 0xE3A01040);
			Process::Patch(0x1A08CC, 0xE5900000);
			Process::Patch(0x1A08D0, 0xEB14CAC6);
			break;
		}
	}
	
	// ------------------------------------------------------
	// 他の人に押されない
	// ------------------------------------------------------
	void OtherPlayersCantPushYou(MenuEntry *entry)
	{
		Keyboard k("", {"オン", "オフ"});
		
		switch(k.Open())
		{
			case 0:
				Process::Write8(0x650D83, 0xEA);
				break;
			case 1:
				Process::Write8(0x650D83, 0x2A);
				break;
		}
	}
	
	// ------------------------------------------------------
	// ゲーム速度上昇
	// ------------------------------------------------------
	void FastGameSpeed(MenuEntry *entry)
	{
		Keyboard k("", {"オン", "オフ"});
		switch(k.Open())
		{
			case 0:
			{
				Process::Write32(0x54c6e8, 0xe3e004ff);
				break;
			}
			case 1:
			{
				Process::Write32(0x54c6e8, 0xe59400a0);
				break;
			}
		}
	}
	
	// ---------------------------------------------------------------
	// 天気変更
	// ---------------------------------------------------------------
	void ChangeWeather(MenuEntry* e)
	{
		Keyboard key("",
			{
				"晴れ(雲なし)",
				"晴れ(雲あり)",
				"くもり空",
				"雨(小)",
				"雨",
				"雪(小)",
				"雪",
			});
		
		int r = key.Open();
		
		if (r >= 0)
		{
			Process::Write32(0x0062E728, 0xE3A00000 + r);
			Process::Write32(0x00949530, 0x01000000 * r);
		}
	}
	
	// ---------------------------------------------------------------
	// 関数呼び出し
	// ---------------------------------------------------------------
	void CallFunction(MenuEntry *e)
	{
		if ( ! Controller::IsKeysDown(A + DD) ) return;
		
		Keyboard	key1( "関数のアドレスを入力" );
		Keyboard	key2( "パラメータの個数を入力(最大11個)" );
		key2.IsHexadecimal(false);
		
		u32		Params[20], ReturnedValue;
		u32		FuncAddress;
		u8		ParamCount;
		std::string	CheckMsg = "";
		
		// Function Address
		if ( key1.Open(FuncAddress) < 0 )
			return;
		else if ( FuncAddress < 0 || FuncAddress > 0x900000 )
			return;
		CheckMsg = Utils::Format("呼び出すアドレス: %08X\n16進数値: %08X\n", FuncAddress, *(u32*)FuncAddress);
		Sleep(Milliseconds(50));
		
		// Parameter Count
		if ( key2.Open(ParamCount) < 0 )
			return;
		else if ( ParamCount > 11 )
			return;
		CheckMsg += Utils::Format("パラメータ数: %d\n\n呼び出し: Func(", ParamCount);
		Sleep(Milliseconds(50));
		
		// Parameter
		for (int i = 0; i < ParamCount; i++)
		{
			Keyboard key3(Utils::Format("パラメータに渡す値を入力\n\nパラメータ%d:", i));
			if (key3.Open(Params[i]) < 0) return;
			CheckMsg += Utils::Format("0x%X", Params[i]);
			if (i < (ParamCount - 1)) CheckMsg += ", ";
			Sleep( Milliseconds(50) );
		}
		CheckMsg += ");";
		
		// 確認
		MessageBox msg("確認", "この設定で呼び出します．よろしいですか？\n\n" + CheckMsg);
		if (msg() == false) return;
		
		Sleep( Milliseconds(100) );
		
		// 呼び出す
		CallFuncWrapper cFunc( FuncAddress );
		ReturnedValue = cFunc.WithParamArray(ParamCount, Params);
		
		// 戻り値
		(MessageBox(Utils::Format("戻り値: %X", ReturnedValue)))();
		OSD::Notify(Utils::Format("returned value: %X", ReturnedValue));
		
		
	}
	
	
	// ---------------------------------------------------------------
	// アドレス監視
	// ---------------------------------------------------------------
	void ViewAddress(MenuEntry* e)
	{
		int r0, r1;

		// アドレス
		static u32 addr = 0x33099e50;

		// アドレスリスト
		static std::vector<u32> addrs = { 0x33099e50, 0x33099e58 };

		// アドレス変更
		if (Controller::IsKeysPressed(e->Hotkeys[0].GetKeys()))
		{
			Keyboard key("アドレス変更");
			key.Open(addr);
		}

		// リストに追加
		if (Controller::IsKeysPressed(e->Hotkeys[1].GetKeys()))
		{
			if (addrs.size() >= 10) {
				(MessageBox("10個まで。"))();
				return;
			}
			addrs.push_back(addr);
		}

		// リスト変更
		if (e->Hotkeys[2].IsDown())
		{
			std::vector<std::string> addrsStr = {};

			for (int i = 0; i < addrs.size(); i++)
				addrsStr.push_back(Utils::ToHex(addrs[i]));

			addrsStr.push_back("すべて削除");

			Keyboard key("リスト変更", addrsStr);
			r0 = key.Open();
			OSD::SwapBuffers();

			// すべて削除
			if (r0 == addrs.size())
				addrs.clear();
			// 変更、または削除
			else if (r0 >= 0)
			{
				int number = r0;

				Keyboard key("", { "変更", "削除" });
				r0 = key.Open();

				OSD::SwapBuffers();

				if (r0 == 0)
				{
					Keyboard change("");
					change.Open(addrs[number], addrs[number]);
				}
				else if (r0 == 1)
				{
					addrs.erase(addrs.begin() + (number - 1));
				}
			}
		}

		_OSDManager& manager = OSDManager;
		manager.Lock();
		const Screen &scr = OSD::GetTopScreen();

		u32 value;
		Process::Read32(addr, value);
		scr.Draw(Utils::ToHex(addr) + ": " + Utils::ToHex(value), 4, 0);
		if (addrs.size() > 0)
		{
			for (int i = 0; i < addrs.size(); i++)
			{
				Process::Read32(addrs[i], value);
				scr.Draw(Utils::Format("(%d)%08X: %08X", i, addrs[i], value), 0, 14 + i * 10);
			}
		}

		manager.Unlock();
	}


	// ---------------------------------------------------------------
	// HEXエディタ
	// ---------------------------------------------------------------
	void HexEditor(MenuEntry *entry)
	{
		static u32 address = 0x33099e50;
		u32 edit_value, jump_addr;
		const int view_length = 10;
		static int sx, sy;
		u32 tmp;
		
		u32 now_addr = address + sy * 0x10 + sx * 0x4;
		
		// Xをおしながら十字キーで編集の場所変更
		if ( Controller::IsKeyDown(X) )
		{
			if ( Controller::IsKeyPressed(DU) ) sy--;
			if ( Controller::IsKeyPressed(DD) ) sy++;
			if ( Controller::IsKeyPressed(DL) ) sx--;
			if ( Controller::IsKeyPressed(DR) ) sx++;
			
			if ( sx < 0 ) sx = 0;
			if ( sx > 3 ) sx = 3;
			
			if ( sy < 0 ) sy = 0;
			if ( sy > 9 ) sy = 9;
		}
		
		// Yを押しながら十字キーの上/下でスクロール
		if ( Controller::IsKeyDown(Y) )
		{
			if ( Controller::IsKeyPressed(DU) ) address -= 0x10;
			if ( Controller::IsKeyPressed(DD) ) address += 0x10;
		}
		
		// ジャンプ
		if ( Controller::IsKeysDown(X + Y) )
		{
			Keyboard key("ジャンプ");
			if ( key.Open( jump_addr, address ) == 0 )
			{
				address = jump_addr;
			}
		}
		
		// 編集
		if ( Controller::IsKeyPressed(A) )
		{
			Keyboard key("値を編集");
			if ( key.Open( edit_value, *(u32 *)(now_addr) ) == 0 )
			{
				Process::Write32(now_addr, edit_value);
			}
		}
		
		_OSDManager& manager = OSDManager;
		manager.Lock();
		
		const Screen &scr = OSD::GetTopScreen();
		
		for ( int i = 0; i < view_length; i++ )
		{
			scr.Draw(Utils::Format("%08X:", address + i * 0x10), 0, i * 10);
			for ( int x = 0; x < 4; x++ )
			{
				Color c = Color::Black;
				if ( x == sx && i == sy ) c = Color::Blue;
				
				if ( Process::Read32((address + i * 0x10 + x * 0x4), tmp) )
					scr.Draw(Utils::Format("%08X", tmp), (6 * 10 + 1) + x * ((6 * 9) + 1), i * 10, Color::White, c);
				else
					scr.Draw("????????", (6 * 10 + 1) + x * ((6 * 9) + 1), i * 10, Color::White, c);
			}
		}
		
		manager.Unlock();
	}
	
	
	
	
	
	
	
	
	
}


