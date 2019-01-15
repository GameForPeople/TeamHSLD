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

	enum class ITEM_PRICE : int
	{
		TERRAIN = 10000
		, PLANT = 30000
		, ANIMAL = 50000
	};

	const int			itemCount;
	std::vector<int>	itemPriceCont;

public:
	ItemManager() 
		: itemCount(3)
	{
		itemPriceCont.reserve(itemCount + 1);
		itemPriceCont.emplace_back(0);	// index - 0 buffer;
		itemPriceCont.emplace_back(static_cast<int>(ITEM_PRICE::TERRAIN));
		itemPriceCont.emplace_back(static_cast<int>(ITEM_PRICE::PLANT));
		itemPriceCont.emplace_back(static_cast<int>(ITEM_PRICE::ANIMAL));
	};
	~ItemManager() = default;
public:
	_NODISCARD __inline constexpr int GetItemCount() const noexcept { return itemCount; }
	_NODISCARD __inline constexpr int GetItemPriceWithIndex(const int InIndex) const noexcept { return itemPriceCont[InIndex]; };
};
