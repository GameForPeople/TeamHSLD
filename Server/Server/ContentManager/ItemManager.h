#pragma once

// this head file include in UserData.h
#include "../PCH/stdafx.h"

class ItemManager
{
	enum ITEM_INDEX : int	// for Bit
	{
		TERRAIN = 1
		, PLANT = 2
		, ANIMAL = 3
	};

	enum class ITEM_BIT : int
	{
		ZERO_BUFFER = 0
		, TERRAIN = 0x00000001
		, PLANT = 0x00000002
		, ANIMAL = 0x00000004
	};

	enum class ITEM_PRICE : int
	{
		ZERO_BUFFER = 0
		, TERRAIN = 10000
		, PLANT = 30000
		, ANIMAL = 50000
	};

	const int										itemCount;
	std::vector<std::pair<ITEM_BIT, ITEM_PRICE>>	itemCont;

public:
	ItemManager() 
		: itemCount(3)
	{
		itemCont.reserve(itemCount + 1);
		itemCont.emplace_back(make_pair<ITEM_BIT, ITEM_PRICE>(ITEM_BIT::ZERO_BUFFER, ITEM_PRICE::ZERO_BUFFER));	// index - 0 buffer;
		itemCont.emplace_back(make_pair<ITEM_BIT, ITEM_PRICE>(ITEM_BIT::TERRAIN, ITEM_PRICE::TERRAIN));
		itemCont.emplace_back(make_pair<ITEM_BIT, ITEM_PRICE>(ITEM_BIT::PLANT, ITEM_PRICE::PLANT));
		itemCont.emplace_back(make_pair<ITEM_BIT, ITEM_PRICE>(ITEM_BIT::ANIMAL, ITEM_PRICE::ANIMAL));
	};
	~ItemManager() = default;
public:
	_NODISCARD __inline constexpr int GetItemCount() const noexcept { return itemCount; }
	_NODISCARD __inline ITEM_BIT GetItemBitWithIndex(const int InIndex) const noexcept { return itemCont[InIndex].first; };
	_NODISCARD __inline ITEM_PRICE GetItemPriceWithIndex(const int InIndex) const noexcept { return itemCont[InIndex].second; };
};
