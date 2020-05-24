#pragma once

#include "CTRPluginFramework.hpp"
#include "Game.hpp"
#include <string>
#include <vector>

namespace CTRPluginFramework
{
	class ChatSelector
	{
	public:
		explicit ChatSelector( std::vector<std::string> Items );
		
		std::vector<std::string>	&GetItems();
		void	SetItems( std::vector<std::string> Items );
		
		// アイテム追加
		void	operator += ( std::string item );
		
		void	operator ++ ();	// ページ 次へ
		void	operator -- ();	// ページ 前へ
		
		std::string GetItem( int index );
		int		GetIndex();
		bool	IsSelecting();
		
		void	Clear();
		
		void	Enable();
		void	Disable();
		
	private:
		std::vector<std::string>	_Items;
		bool	_Selecting;
		int		_Index;
	};
}

