#pragma once

namespace CTRPluginFramework
{
	// とび森通貨を複合化する
	u32 DecryptACNLMoney(u64 money);
	
	// 数値をとび森通貨の16進数値に暗号化する
    u64 EncryptACNLMoney(int dec);
	
	// ---------------------------------------------------------------
	// アイテム検索
	// ～アイテム一覧の中から検索して、一致した場合はoutputに追加します。
	// ・str	: 探す文字列
	// ・output	: 追加されるベクター配列(ITEM_MAP)
	// ---------------------------------------------------------------
	void ItemSearch(const std::string &str, std::vector<ITEM_MAP> &output);
	
	
	
}

