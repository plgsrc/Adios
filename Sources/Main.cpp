#include "Cheats.hpp"
#include "TextColors.h"
#include "csvc.h"

void abort(void);

namespace CTRPluginFramework
{
	void Patches(void)
	{
		const u32 nop = 0xE1A00000;
		const u32 dropReplaceSwitchAddr = USA0x005A1120;
		int		choice, code, n;
		u8		*house_base;
		char	buf[300];
		
		// 「ここにはもう置けないヨ」を非表示にする
		Process::Write32(0x19df08, 0xe1a00000);
		
		// inventory base
		Process::Write32( USA0x0019B694, nop );
		Process::Write32( USA0x0019BF2C, nop );
		Process::Write32( USA0x0019BA78, nop );
		Process::Write32( USA0x0019B988, nop );
		Process::Write32( USA0x0019C050, nop );
		Process::Write32( USA0x00725124, nop );
		
		// items2money
		Process::Write32( USA0x0019B5BC, 0xE3A06001 );
		
		// drop
		Process::Write32( USA0x0059FCA4, 0xEA000004 );
		
		// host drop 
        Process::Write32(0x59FD8C, 0xE1A00000); //create locked spot pkt send
        Process::Write32(0x59FC2C, 0xE1A00000); //clear locked spot pkt send
        Process::Write32(0x59E894, 0xEA000010); //pkt is from host
        Process::Write32(0x59E6D0, 0xEA000003); //bypass check in 0x59FC7C
        Process::Write32(0x596D2C, 0xE1A00000); //unnecessary?
        Process::Write32(0x596E14, 0xE1A00000); //unnecessary?
        Process::Write32(0x59E1D8, 0xE1A00000);
        Process::Write32(0x59F2E8, 0xEA000022); //item tree drop
        Process::Write32(0x59F72C, 0xEA00002D); //fruit tree drop
        
		// drop everywhere
        Process::Write32(0x1655D4, 0xE3A00001);
        Process::Write32(0x1655E0, 0xEA000006);
        Process::Write32(0x1654D4, 0xEA000005);
        Process::Write32(0x165568, 0xEA000010); // on other players
		
        //some drop things
        for (int i = 0; i <= 0x21; i++)
			Process::Write32(0x59F88C + i * 4, 0x59FA58);
		
		Process::Write32( 0x19DF08, 0xE1A00000 ); // ここには置けない表示しない
		Process::Write32( 0x7238C0, 0xE1A00000 ); // 没アイテム表示させる
		Process::Write32( 0x19C150, 0xE1A00000 ); // 
		Process::Write32( 0x19BFC8, 0xE1A00000 ); //
		Process::Write32( 0x19BFB0, 0xE1A00000 ); //
		Process::Write32( 0x68B128, 0xE1A00000 ); // アクセサリーとヘルメット同時使用可能
		
		// ラグなし
		Process::Write32( 0x0059E6D0, 0xEA000003 );
		Process::Write32( 0x0059E894, 0xEA000010 );
		Process::Write32( 0x0059FD8C, 0xE1A00000 );
		
		// 壁抜け
		Process::Write32(0x0064EEF4, 0xEA000094);
		Process::Write32(0x0064EF0C, 0xEA000052);
		Process::Write32(0x0064F070, 0xEA000001);
		Process::Write32(0x0064F0E8, 0xEA000014);
		Process::Write32(0x0064F19C, 0xE1A00000);
		Process::Write32(0x0064F1B4, 0xE1A00000);
		Process::Write32(0x0064F1B8, 0xEA000026);
		Process::Write32(0x0064F1E4, 0xEA000065);
		
		// 他の人に押されない
		Process::Write8(0x650D83, 0xEA);
		
		// セーブメニュー非表示
		Process::Patch(0x1A08C8, 0xE1A00000);
		Process::Patch(0x1A08CC, 0xE3A00000);
		Process::Patch(0x1A08D0, 0xEB0E011D);
		
		
	}
	
	MenuEntry* EntryWithHotkey(MenuEntry* entry, const Hotkey& hotkey)
	{
		if (entry != nullptr)
		{
			entry->Hotkeys += hotkey;
			entry->SetArg(new std::string(entry->Name()));
			entry->Name() += " " + hotkey.ToString();
			entry->Hotkeys.OnHotkeyChangeCallback([](MenuEntry* entry, int index)
				{
					std::string* name = reinterpret_cast<std::string*>(entry->GetArg());
					entry->Name() = *name + " " + entry->Hotkeys[0].ToString();
				});
		}
		return (entry);
	}

	MenuEntry* EntryWithHotkey(MenuEntry* entry, const std::vector<Hotkey>& hotkeys)
	{
		if (entry != nullptr)
		{
			for (const Hotkey& hotkey : hotkeys)
				entry->Hotkeys += hotkey;
		}
		return (entry);
	}
	
	template <typename T>
	MenuEntry *EntryWithArg(MenuEntry *entry, T defaultValue)
	{
		if( entry != nullptr )
			entry->SetArg( new T(defaultValue) );
		
		return entry;
	}
	
	void InitEntrys(PluginMenu& menu)
	{
		MenuEntry *t2c = new MenuEntry("常時オン", TextToCheats,
					"コマンド一覧\n"
					"\n"
					"tchdrop on\n"
					"tchdrop off\n"
					"tchdrop\n"
					"del all// 地上のアイテム全部消す\n"
					"delete// 足元のアイテムを消す\n"
					"(アイテム名)// アイテムを検索する");
		
		t2c->Enable();
		menu += t2c;
		
		menu += new MenuEntry("アイテムID検索", nullptr, MenuItemSearcher);
		
		/**
		 *	プレイヤー
		 **************************/
		menu += new MenuFolder(Color(ccSkyBlue) << "*\uE050" << Color::SkyBlue << " プレイヤー" << Color(ccSkyBlue) << " \uE051*", "",
		{
			new MenuEntry(Color::SkyBlue << "\uE050 プレイヤー変更", nullptr, ChangePlayer),
			new MenuEntry(Color::SkyBlue << "\uE050 エリア移動", nullptr, ChangeArea),
			new MenuEntry(Color::SkyBlue << "\uE050 名前変更", nullptr, ChangeName),
			new MenuEntry(Color::SkyBlue << "\uE050 国籍変更", nullptr, ChangeRegion),
			new MenuEntry(Color::SkyBlue << "\uE050 性別変更", nullptr, ChangeDenger),
			new MenuEntry(Color::SkyBlue << "\uE050 バッジ変更", nullptr, ChangeBadge),
			
		});
		
		/**
		 *	遊び用
		 **************************/
		MenuEntry *item_chg = new MenuEntry(Color(ccNormalOrange) << "\uE015 アイテム変更 \uE003 + \uE07C", ChangeUseItem, ChangeUseItem_Option);
		item_chg->Enable();
		menu += new MenuFolder(Color(ccSkyBlue) << "*\uE015" << Color(ccNormalOrange) << " 遊び用" << Color(ccSkyBlue) << " \uE015*", "",
		{
			item_chg,
			new MenuEntry(Color(ccNormalOrange) << "\uE015 アイテムランダマイザ \uE004", ItemRandomizer),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 マップエディタ START + \uE079", MapEditor),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 自動ドロップ \uE003 + \uE07B", AutoDrop, AutoDrop_Option),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 タッチドロップ", TouchDrop),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 タッチアイテム削除", TouchTrample),
			//new MenuEntry(Color(ccNormalOrange) << "\uE015 パーティクル \uE000 + \uE079", ShowParticles),
			//new MenuEntry(Color(ccNormalOrange) << "\uE015 タッチパーティクル \uE058", TouchParticles),
			
		});
		
		/**
		 *	インベントリ
		 **************************/
		menu += new MenuFolder(Color(ccSkyBlue) << "*\uE016" << Color::SkyBlue << " インベントリ " << Color(ccSkyBlue) << "\uE016*", "",
		{
			new MenuEntry(Color::SkyBlue << "\uE016 未使用アイテム表示", nullptr, DisableItemLocks),
			new MenuEntry(Color::SkyBlue << "\uE016 置いても無くならない", nullptr, InfinityItem),
			new MenuEntry(Color::SkyBlue << "\uE016 すべて削除", nullptr, DeleteInvItems),
			new MenuEntry(Color::SkyBlue << "\uE016 選択肢変更", nullptr, InvItemOption),
			new MenuEntry(Color::SkyBlue << "\uE016 所持金変更", nullptr, ChangeWalletBell),
			new MenuEntry(Color::SkyBlue << "\uE016 アイテム取得 \uE002 + \uE07C", TextToItem),
			
		});
		
		/**
		 *	動き，操作
		 **************************/
		MenuFolder *Movements = new MenuFolder(Color(ccSkyBlue) << "*\uE017" << Color(ccNormalOrange) << " 動き，操作" << Color(ccSkyBlue) << " \uE017*");
		{
			*Movements += InitActionsFolder( Color(ccNormalOrange) << "\uE017 アクション" );// アクション
			*Movements += EntryWithArg(new MenuEntry(Color(ccNormalOrange) << "\uE017 座標移動 \uE000 + \uE006", CoordinateModifier, ChangeCoordSpeed), 8.0f);
			*Movements += new MenuEntry(Color(ccNormalOrange) << "\uE017 壁抜け \uE004 + \uE079", WalkOverObjects);
			*Movements += new MenuEntry(Color(ccNormalOrange) << "\uE017 タッチワープ \uE058", TouchWarping);
			*Movements += new MenuEntry(Color(ccNormalOrange) << "\uE017 アニメーション実行 \uE000 + \uE079", ExecAnimation, "アニメID変更: \uE002");
			*Movements += new MenuEntry(Color(ccNormalOrange) << "\uE017 他の人を自分の場所に集める \uE004 + \uE07C", PlayerTeleporter);
		}
		menu += Movements;
		
		/**
		 *	オン島
		 **************************/
		menu += new MenuFolder(Color(ccSkyBlue) << "*\uE008" << Color::SkyBlue << " オン島" << Color(ccSkyBlue) << " \uE009*", "",
		{
			new MenuEntry(Color::SkyBlue << "\uE008 メダルの枚数変更", nullptr, ChangeIslandMedal),
			
		});
		
		/**
		 *	無人島ゲーム
		 **************************/
		menu += new MenuFolder(Color(ccSkyBlue) << "*\uE015" << Color(ccNormalOrange) << " 無人島ゲーム" << Color(ccSkyBlue) << " \uE015*", "",
		{
			new MenuEntry(Color(ccNormalOrange) << "\uE015 歩数変更P1", nullptr, mujintou_hosuuP1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 歩数変更P2", nullptr, mujintou_hosuuP2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 歩数変更P3", nullptr, mujintou_hosuuP3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 歩数固定", hosuukotei),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 歩いた時のスコア変更", nullptr, mujintou_WalkScore),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 歩いた時のスコア固定", Walkscore_kotei),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 ステージ開放率変更", nullptr, mujinntou_stage_open),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 棒所持数変更", nullptr, mujintou_Item1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 糸所持数変更", nullptr, mujintou_Item2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 布きれ所持数変更", nullptr, mujintou_Item3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 石ころ所持数変更", nullptr, mujintou_Item4),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 ぎんこうせき所持数変更", nullptr, mujintou_Item5),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 きんこうせき所持数変更", nullptr, mujintou_Item6),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 おくすり所持数変更", nullptr, mujintou_Item7),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 丸太の入手数変更", nullptr, mujintou_hoof1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 船の旗の入手数変更", nullptr, mujintou_hoof2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 丸太の残りの数変更", nullptr, mujintou_hoof3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 船の旗の残りの数変更", nullptr, mujintou_hoof4),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 釣り竿所持変更", nullptr, mujintou_Tool1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 パチンコ所持変更", nullptr, mujintou_Tool2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 網所持変更", nullptr, mujintou_Tool3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 スコップ所持変更", nullptr, mujintou_Tool4),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 銀の釣り竿所持変更", nullptr, mujintou_STool1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 銀のパチンコ所持変更", nullptr, mujintou_STool2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 銀の網所持変更", nullptr, mujintou_STool3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 銀のスコップ所持変更", nullptr, mujintou_STool4),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 金の釣り竿所持変更", nullptr, mujintou_GTool1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 金のパチンコ所持変更", nullptr, mujintou_GTool2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 金の網所持変更", nullptr, mujintou_GTool3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 金のスコップ所持変更", nullptr, mujintou_GTool4),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 残り日数変更", nullptr, mujintou_hizuke1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 経過日数変更", nullptr, mujintou_hizuke2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 食料落下数変更", nullptr, mujintou_syokuryou1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 リンゴゲージ変更", nullptr, mujintou_syokuryou2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 食料所持数変更", nullptr, mujintou_syokuryou3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 落とし穴無効化", nullptr, mujintou_Otosiana),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 特技変更P1", nullptr, mujintou_TokugiP1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 特技変更P2", nullptr, mujintou_TokugiP2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 特技変更P3", nullptr, mujintou_TokugiP3),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 好きな食べ物変更P1", nullptr, mujintou_FoodP1),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 好きな食べ物変更P2", nullptr, mujintou_FoodP2),
			new MenuEntry(Color(ccNormalOrange) << "\uE015 好きな食べ物変更P3", nullptr, mujintou_FoodP3),
		});
		
		/**その他
		 **************************/
		menu += new MenuFolder(Color(ccSkyBlue) << "*\uE018" << Color::SkyBlue << " その他" << Color(ccSkyBlue) << " \uE018*", "",
		{
			new MenuEntry(Color::SkyBlue << "\uE018 セーブメニュー非表示", nullptr, DisableSaveMenu),
			new MenuEntry(Color::SkyBlue << "\uE018 他の人に押されない", nullptr, OtherPlayersCantPushYou),
			new MenuEntry(Color::SkyBlue << "\uE018 ゲーム速度上昇", nullptr, FastGameSpeed),
			 EntryWithHotkey(new MenuEntry(Color::SkyBlue << "\uE018 アドレス監視", ViewAddress),
			{
				Hotkey(Key::R + A, "アドレス変更"),
				Hotkey(Key::R + B, "リストに追加"),
				Hotkey(Key::R + X, "リスト変更"),
			}),
			new MenuEntry(Color::SkyBlue << "\uE018 関数呼び出し \uE000 + \uE07A", CallFunction),
			new MenuEntry(Color::SkyBlue << "\uE018 HEXエディタ", HexEditor),
			
			
		});
		
		
		
		
		
		
	}
	
	bool ChangeUseItem_OSDcb(const Screen &s);
	int main(void)
	{
		if ( Process::GetTitleID() != 0x0004000000086200 )
			return 0;
		
		Patches();
		PluginMenu *menu =
			new PluginMenu(Color(ccSkyBlue) << "+\uE018  " << Color::Red<<"A"<<Color::ForestGreen<<"d"<<Color::Yellow<<"i"<<Color::DodgerBlue<<"o"<<Color(ccNormalPurple)<<"s" << Color(ccSkyBlue) << "  \uE018+", 3,0,4);
		
		menu->SynchronizeWithFrame(true);
		menu->ShowWelcomeMessage(false);
		
		OSD::Run( ChangeUseItem_OSDcb );
		OSD::Run( UniNotify_Callback );
		UnicodeNotify("Plugin ready!");
		
		InitEntrys(*menu);
		menu->Run();

		delete menu;
		return 0;
	}
}




