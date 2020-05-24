#pragma once

#include "3ds.h"
#include <string>
#include <vector>
#include "CTRPluginFramework.hpp"
#include "MenuEntryHelpers.hpp"
#include "OSDManager.hpp"
#include "Address.h"
#include "Game.hpp"
#include "maps.hpp"
#include "Helpers.hpp"

namespace CTRPluginFramework
{
	using namespace std;
	using StringVector = vector<string>;
	
	// UnicodeNotify
	void UnicodeNotify(const string &text, Color fore = Color::White, Color back = Color::Black);
	bool UniNotify_Callback(const Screen &scr);
	
	void TextToCheats(MenuEntry *entry);
	void MenuItemSearcher(MenuEntry *entry);
	
	/**
	 *	プレイヤー
	 **************************/
	void ChangePlayer(MenuEntry*);
	void ChangeArea(MenuEntry*);
	void ChangeName(MenuEntry*);
	void ChangeRegion(MenuEntry*);
	void ChangeDenger(MenuEntry*);
	void ChangeBadge(MenuEntry*);
	
	/**
	 *	遊び用
	 **************************/
	void ChangeUseItem(MenuEntry *entry);
	void ChangeUseItem_Option(MenuEntry *entry);
	void ItemRandomizer(MenuEntry *entry);
	void MapEditor(MenuEntry *entry);
	void AutoDrop(MenuEntry *entry);
	void AutoDrop_Option(MenuEntry *entry);
	void TouchDrop(MenuEntry *entry);
	void TouchTrample(MenuEntry *entry);
	void ShowParticles(MenuEntry *entry);
	void TouchParticles(MenuEntry *entry);
	
	/**
	 *	インベントリ
	 **************************/
	void DisableItemLocks(MenuEntry*);		// 未使用アイテムを表示
	void InfinityItem(MenuEntry*);			// 置いても無くならない
	void DeleteInvItems(MenuEntry*);		// すべて削除
	void InvItemOption(MenuEntry*);			// 選択肢変更
	void TextToItem(MenuEntry *entry);		// アイテム取得
	void ChangeWalletBell(MenuEntry *entry);// 所持金変更
	
	/**
	 *	動き，操作
	 **************************/
		// アクション
		MenuFolder *InitActionsFolder(const std::string &name);
	void CoordinateModifier(MenuEntry *entry);
	void ChangeCoordSpeed(MenuEntry *entry);
	void WalkOverObjects(MenuEntry *entry);
	void TouchWarping(MenuEntry *entry);
	void ExecAnimation(MenuEntry *entry);
	void PlayerTeleporter(MenuEntry *entry);
	
	/**
	 *	オン島
	 **************************/
	void ChangeIslandMedal(MenuEntry *entry);
	
	/**
     *	無人島
	 **************************/
	void mujintou_hosuuP1(MenuEntry*);		// 歩数変更P1
	void mujintou_hosuuP2(MenuEntry*);		// 歩数変更P2
	void mujintou_hosuuP3(MenuEntry*);		// 歩数変更P3
	void hosuukotei(MenuEntry*);			// 歩数固定
	void mujinntou_stage_open(MenuEntry*);	// 歩いた時のスコア変更
	void Walkscore_kotei(MenuEntry*);		// 歩いた時のスコア固定
	void mujintou_WalkScore(MenuEntry*);	// ステージ開放率変更
	void mujintou_Item1(MenuEntry*);		// 棒所持数変更
	void mujintou_Item2(MenuEntry*);		// 糸所持数変更
	void mujintou_Item3(MenuEntry*);		// 布きれ所持数変更
	void mujintou_Item4(MenuEntry*);		// 石ころ所持数変更
	void mujintou_Item5(MenuEntry*);		// ぎんこうせき所持数変更
	void mujintou_Item6(MenuEntry*);		// きんこうせき所持数変更
	void mujintou_Item7(MenuEntry*);		// おくすり所持数変更
	void mujintou_hoof1(MenuEntry*);		// 丸太の入手数変更
	void mujintou_hoof2(MenuEntry*);		// 船の旗の入手数変更
	void mujintou_hoof3(MenuEntry*);		// 丸太の残りの数変更
	void mujintou_hoof4(MenuEntry*);		// 船の旗の残りの数変更
	void mujintou_Tool1(MenuEntry*);		// 釣り竿所持変更
	void mujintou_Tool2(MenuEntry*);		// パチンコ所持変更
	void mujintou_Tool3(MenuEntry*);		// 網所持変更
	void mujintou_Tool4(MenuEntry*);		// スコップ所持変更
	void mujintou_STool1(MenuEntry*);		// 銀の釣り竿所持変更
	void mujintou_STool2(MenuEntry*);		// 銀のパチンコ所持変更
	void mujintou_STool3(MenuEntry*);		// 銀の網所持変更
	void mujintou_STool4(MenuEntry*);		// 銀のスコップ所持変更
	void mujintou_GTool1(MenuEntry*);		// 金の釣り竿所持変更
	void mujintou_GTool2(MenuEntry*);		// 金のパチンコ所持変更
	void mujintou_GTool3(MenuEntry*);		// 金の網所持変更
	void mujintou_GTool4(MenuEntry*);		// 金のスコップ所持変更
	void mujintou_hizuke1(MenuEntry*);		// 残り日数変更
	void mujintou_hizuke2(MenuEntry*);		// 経過日数変更
	void mujintou_syokuryou1(MenuEntry*);	// 食料落下数変更
	void mujintou_syokuryou2(MenuEntry*);	// リンゴゲージ変更
	void mujintou_syokuryou3(MenuEntry*);	// 食料所持数変更
	void mujintou_Otosiana(MenuEntry*);		// 落とし穴無効化
	void mujintou_TokugiP1(MenuEntry*);		// 特技変更P1
	void mujintou_TokugiP2(MenuEntry*);		// 特技変更P2
	void mujintou_TokugiP3(MenuEntry*);		// 特技変更P3
	void mujintou_FoodP1(MenuEntry*);		// 好きな食べ物変更P1
	void mujintou_FoodP2(MenuEntry*);		// 好きな食べ物変更P2
	void mujintou_FoodP3(MenuEntry*);		// 好きな食べ物変更P3
	
	
	/**
	 *	その他
	 **************************/
	void DisableSaveMenu(MenuEntry *entry);
	void OtherPlayersCantPushYou(MenuEntry *entry);
	void FastGameSpeed(MenuEntry *entry);
	void TCP_ImageEditor(MenuEntry *entry);
	void CallFunction(MenuEntry *e);
	void ViewAddress(MenuEntry* e);
	void HexEditor(MenuEntry *entry);
	
	
	
	
	
}






