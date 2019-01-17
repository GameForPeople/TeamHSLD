#pragma once

// this head file include in UserData.h
#include "../PCH/stdafx.h"

class CharacterManager
{
	enum CHARACTER_INDEX : int	// for Bit
	{
		MY_PLANET = 0
		, VICTORY = 1
		, SUN = 2
		, SOIL = 3
		, GRASS = 4
		, WATER = 5
		, FIRE = 6
		, ICE = 7
		, TREE = 8
	};

	enum class CHARACTER_PRICE : int
	{
		MY_PLANET = 0
		, VICTORY = 1000
		, SUN = 1000
		, SOIL = 1000
		, GRASS = 1000
		, WATER = 1000
		, FIRE = 1000
		, ICE = 1000
		, TREE = 1000
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