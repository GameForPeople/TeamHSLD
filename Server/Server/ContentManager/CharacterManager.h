#pragma once

// this head file include in UserData.h
#include "../PCH/stdafx.h"

class CharacterManager
{
public:
	enum CHARACTER_INDEX : int
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

	enum class CHARACTER_BIT : int
	{
		MY_PLANET = 0
		, VICTORY = 1 << 0
		, SUN = 1 << 1
		, SOIL = 1 << 2
		, GRASS = 1 << 3
		, WATER = 1 << 4
		, FIRE = 1 << 5
		, ICE = 1 << 6
		, TREE = 1 << 7
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

private:
	const int			characterCount;
	std::vector<std::pair<CHARACTER_BIT,  CHARACTER_PRICE>>	characterCont;

public:
	CharacterManager()
		: characterCount(8)
	{
		characterCont.reserve(characterCount + 1);
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::MY_PLANET, CHARACTER_PRICE::MY_PLANET));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::VICTORY, CHARACTER_PRICE::VICTORY));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::SUN, CHARACTER_PRICE::SUN));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::SOIL, CHARACTER_PRICE::SOIL));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::GRASS, CHARACTER_PRICE::GRASS));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::WATER, CHARACTER_PRICE::WATER));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::FIRE, CHARACTER_PRICE::FIRE));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::ICE, CHARACTER_PRICE::ICE));
		characterCont.emplace_back(std::make_pair/*<CHARACTER_BIT, CHARACTER_PRICE>*/(CHARACTER_BIT::TREE, CHARACTER_PRICE::TREE));
	};
	~CharacterManager() = default;
public:
	_NODISCARD __inline constexpr int GetCharacterCount() const noexcept { return characterCount; }
	_NODISCARD __inline CHARACTER_BIT GetCharacterBitWithIndex(const int InIndex) const noexcept { return characterCont[InIndex].first; };
	_NODISCARD __inline CHARACTER_PRICE GetCharacterPriceWithIndex(const int InIndex) const noexcept { return characterCont[InIndex].second; };
};