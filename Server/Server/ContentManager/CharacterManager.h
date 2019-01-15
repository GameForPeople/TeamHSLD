#pragma once

// this head file include in UserData.h
#include "../PCH/stdafx.h"

class CharacterManager
{
	enum CHARACTER_INDEX : int	// for Bit
	{
			VICTORY = 1
		,	SOIL = 2
		,	GRASS = 3
		,	WATER = 4
		,	FIRE = 5
		,	ICE = 6
		,	TREE = 7
	};

	enum class CHARACTER_PRICE : int
	{
		VICTORY = 10000000000
		, SOIL = 10000
		, GRASS = 10000
		, WATER = 10000
		, FIRE = 10000
		, ICE = 10000
		, TREE = 10000
	};

	const int			characterCount;
	std::vector<int>	characterPriceCont;

public:
	CharacterManager()
		: characterCount(7)
	{
		characterPriceCont.reserve(characterCount + 1);
		characterPriceCont.emplace_back(0);	// index - 0 buffer;
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::VICTORY));
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::SOIL));
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::GRASS));
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::WATER));
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::FIRE));
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::ICE));
		characterPriceCont.emplace_back(static_cast<int>(CHARACTER_PRICE::TREE));
	};
	~CharacterManager() = default;
public:
	_NODISCARD __inline constexpr int GetCharacterCount() const noexcept { return characterCount; }
	_NODISCARD __inline int GetCharacterPriceWithIndex(const int InIndex) const noexcept { return characterPriceCont[InIndex]; };
};