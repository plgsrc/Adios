#include "Cheats.hpp"
#include "Game/Items.h"

namespace CTRPluginFramework
{
	u16				U16_ChrArray[50];	// キーボード文字用
	bool			KatakanaMode;		// カタカナか否か
	bool			KeyboardOpened;		// キーボード開いているかどうか( =ループを回すか )
	
	std::vector<u16>	InputChrs;	// UTF16文字コードで入力中の文字列を管理する，stringはUTF8なのでこうする必要がある
	std::string			InputStr;	// 上のInputChrsをUTF8stringに変換する為の出力先（Screen::DrawSysfontに引数として渡すため）
	
	const std::string Hiragana =
		"わらやまはなたさかあ"
		"をりゆみひにちしきい"
		"んるよむふぬつすくう"
		"、れ！めへねてせけえ"
		"。ろ？もほのとそこお";
	
	const std::string Katakana =
		"ワラヤマハナタサカア"
		"ヲリユミヒニチシキイ"
		"ンルヨムフヌツスクウ"
		"、レ！メヘネテセケエ"
		"。ロ？モホノトソコオ";
	
	// 	上の文字列達を U16_ChrArray に入れる ( 同時にUTF16に変換する )
	void MakeU16Array()
	{
		if( !KatakanaMode )// ひらがな
			Process::WriteString( (u32)U16_ChrArray, Hiragana, StringFormat::Utf16 );
		else// カタカナ
			Process::WriteString( (u32)U16_ChrArray, Katakana, StringFormat::Utf16 );
			// WriteStringで文字コードを変換しながらアドレスに書き込む事が出来るのでこれを使う
	}
	
	// 指定した範囲をタッチしているかどうか
	// x, y		= 開始地点
	// w, h		= 幅，高さ
	bool TouchRect(u32 x, u32 y, u32 w, u32 h)
	{
		if( Touch::IsDown() )
		{
			UIntVector pos = Touch::GetPosition();
			if( pos.x >= x && pos.y >= y && pos.x <= (x + w) && pos.y <= (y + h) )
				return true;
		}
		
		return false;
	}
	
	void Dakuten( bool handakuten, u16 &moji )
	{
		// ひらがな・カタカナじゃなければ帰る
		if( !(moji >= 0x3042 && moji <= 0x3093) && !(moji >= 0x30A2 && moji <= 0x30F3) ) return;
		
		// 濁点用
		const std::string str1 = "かきくけこさしすせそたちつてとはひふへほ";
		const std::string str2 = "がぎぐげござじずぜぞだぢづでどばびぶべぼ";
		
		// 半濁点用
		const std::string str3 = "はひふへほ";
		const std::string str4 = "ぱぴぷぺぽ";
		
		// UTF16文字配列
		u16 u16array1[100] = {0};// 変換前
		u16 u16array2[100] = {0};// 変換後
		
		// moji = カタカナ
		bool isKatakana = moji >= 0x30A2 && moji <= 0x30F3;
		
		if( handakuten ) // 半濁点モード
		{
			Process::WriteString( (u32)u16array1, str3, StringFormat::Utf16 );
			Process::WriteString( (u32)u16array2, str4, StringFormat::Utf16 );
		}
		else // 濁点モード
		{
			Process::WriteString( (u32)u16array1, str1, StringFormat::Utf16 );
			Process::WriteString( (u32)u16array2, str2, StringFormat::Utf16 );
		}
		
		// 変換
		for( int i = 0; i < (handakuten ? 5 : 20); i++ )
		{
			if( isKatakana ) // カタカナだったら変換用配列をカタカナにする
			{
				u16array1[i] += 0x60;
				u16array2[i] += 0x60;
			}
			
			// ヒットしたら書き換えて終了
			if( u16array1[i] == moji )
			{
				moji = u16array2[i];
				break;
			}
			else if( u16array2[i] == moji )
			{
				moji = u16array1[i];
				break;
			}
		}
	}
	
	void DrawKeyboard(const Screen &scr)
	{
		MakeU16Array();
		
		// 背景
		scr.DrawRect(20, 20, 280, 200, Color::Black);
		scr.DrawRect(22, 22, 276, 196, Color::White, false);
		//scr.DrawRect(40, 68, 240, 110, Color(222, 170, 99));
		
		// 入力
		if( Controller::IsKeyPressed(Touchpad) )
		{
			UIntVector pos = Touch::GetPosition();
			if( pos.x >= 23 && pos.y >= 69 && pos.x <= 262 && pos.y <= 178 )
			{
				int wx = pos.x - 23;
				int wy = pos.y - 69;
				wx /= 24;
				wy /= 22;
				scr.DrawRect(23 + wx * 24, 68 + wy * 22, 24, 22, Color::White);// 打ってるとこを白くする
				
				// 15文字以内だったら，文字を打つ
				if( InputChrs.size() < 15 )
					InputChrs.push_back( U16_ChrArray[wy * 10 + wx] );
			}
		}
		
		// キー描画
		std::string key;
		for( u8 x = 0; x < 10; x++ )
			for( u8 y = 0; y < 5; y++ )
			{
				u32 sx = 23 + x * 24;
				u32 sy = 68 + y * 22;
				
				// 文字
				key.clear();
				Process::ReadString( (u32)&U16_ChrArray[y * 10 + x], key, 2, StringFormat::Utf16 );
				scr.DrawSysfont( key, sx + 5, sy + 3, Color::White );
			}
		
		// 入力中の文字列描画
		scr.DrawRect(55, 56, 212, 1, Color::White);
		if( !InputChrs.empty() )
		{
			InputStr.clear();
			u16 str[100] = {0};
			for( int i = 0; i < InputChrs.size(); i++ )
				str[i] = InputChrs[i];
			Process::ReadString( (u32)str, InputStr, InputChrs.size() * 2, StringFormat::Utf16 );
			
			scr.DrawSysfont(InputStr, 58, 38);
		}
		
		// 右側のオプション
		const std::string opt[5] = { "削除", "かな", "カナ", "”", "゜" };
		for( int i = 0; i < 5; i++ )
		{
			// タッチしてたら
			if( Controller::IsKeyPressed(Touchpad) && TouchRect(268, 72 + i * 22, 24, 16) )
			{
				switch( i )
				{
					case 0:// 文字削除
						if( !InputChrs.empty() ) InputChrs.pop_back();
						break;
					case 1:// カタカナオフ
						KatakanaMode = false;
						break;
					case 2:// カタカナオン
						KatakanaMode = true;
						break;
					case 3:// 濁点　”
						Dakuten(false, InputChrs[ InputChrs.size() - 1 ]);
						break;
					case 4:// 半濁点  ゜
						Dakuten(true, InputChrs[ InputChrs.size() - 1 ]);
						break;
				}
				scr.DrawRect( 263, 68 + i * 22, 34, 22, Color::White );
			}
			scr.DrawSysfont( opt[i], 268, 72 + i * 22 );
		}
		
		// とじる
		scr.DrawRect(28, 191, 68, 22, Color::Gray);
		scr.DrawRect(28, 191, 68, 22, Color::White, false);
		scr.DrawSysfont("とじる", 42, 194);
		if( Controller::IsKeyPressed(B) || TouchRect(28, 191, 68, 22) )
			KeyboardOpened = false;
		
		// 検索
		scr.DrawRect(224, 191, 68, 22, Color::Gray);
		scr.DrawRect(224, 191, 68, 22, Color::White, false);
		scr.DrawSysfont("検索", 246, 194);
		if( TouchRect(224, 191, 68, 22) )
		{
			if( !InputStr.empty() )
			{
				std::vector<ITEM_MAP> Hits;
				ItemSearch( InputStr, Hits );
				
				if( !Hits.empty() )
				{
					std::vector<std::string> Names;
					for( const ITEM_MAP &i : Hits )
						Names.push_back(i.name1);
					
					Keyboard key("", Names);
					int res = key.Open();
					
					if( res >= 0 )
					{
						OSD::SwapBuffers();
						Keyboard ItemInfo("",
						{
							"アイテム名: " + Names[res],
							Utils::Format("アイテムID: ", Hits[res].code),
							"ポケットに入れる",
							"戻る"
						});
						
						while( true )
						{
							OSD::SwapBuffers();
							
							int r = ItemInfo.Open();
							if( r == 2 ) if( Player::GetInstance(4, 1) > 0 ) *Player::GetInvSlotAddr(0) = Hits[res].code;
							if( r == 3 ) break;
						}
					}
					
					KeyboardOpened = false;
				}
			}
		}
		
	}
	
	void MenuItemSearcher(MenuEntry *entry)
	{
		// プロセス停止中じゃなければ帰る(OSDからScreen取得のため)
		// CTRPFメニューを開いているときは停止中
		if( !Process::IsPaused() ) return;
		
		InputChrs.clear();	// U16文字列初期化
		InputStr.clear();	// 表示用文字列初期化
		
		// 下画面を取得する
		const Screen &scr = OSD::GetBottomScreen();
		
		KeyboardOpened = true;
		while( KeyboardOpened )// メインループ
		{
			Controller::Update();	// コントローラ更新
			DrawKeyboard(scr);		// キーボード描画
			OSD::SwapBuffers();		// スクリーンフレームバッファ更新
		}
	}
	
	
	
	
	
	
}


