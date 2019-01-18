#include "../UserData/UserData.h"

#include "../IOCPServer/SocketInfo.h"

const wstring UserData::VIP_CODE = VIP_CODE_STRING;
unique_ptr<ItemManager> UserData::itemManager = make_unique<ItemManager>();
unique_ptr<CharacterManager> UserData::characterManager = make_unique<CharacterManager>();

/*
	BuyItem

	인자값 : enum ITEM_INDEX
	반환값 : -1이면 성공, 0이면 돈없어서 실패, 1이면 이미 있는 아이템이여서 실패, 2면 이상한값인데? 실패.
*/
_NODISCARD int UserData::BuyItem(const int InItemIndex)
{
	if (InItemIndex > itemManager->GetItemCount())
	{
		// 그런 아이템 안팔아요.
		return 2;
	}

	if (int retItemBit = static_cast<int>(itemManager->GetItemBitWithIndex(InItemIndex))
		; (itemBit & retItemBit) == retItemBit)
	{
		// 이미 구매한 아이템.
		return 1;
	}
	else if (int itemPrice = static_cast<int>(itemManager->GetItemPriceWithIndex(InItemIndex))
		; money >= itemPrice)
	{
		money -= itemPrice;
		itemBit |= retItemBit;

		return -1;
	}

	// 돈 없나벼.
	return 0;
}

/*
	BuyCharacter

	인자값 : enum ITEM_INDEX
	반환값 : -1이면 성공, 0이면 돈없어서 실패, 1이면 이미 있는 캐릭터이여서 실패, 2면 이상한값인데? 실패.
*/
_NODISCARD int UserData::BuyCharacter(const int InCharacterIndex)
{
	if (InCharacterIndex > characterManager->GetCharacterCount())
	{
		// 그런 캐릭터 안팔아요.
		return 2;
	}

	if (int RetCharacterBit = static_cast<int>(characterManager->GetCharacterBitWithIndex[InCharacterIndex])
		; (characterBit & RetCharacterBit) == RetCharacterBit)
	{
		// 이미 구매한 아이템.
		return 1;
	}
	else if (int characterPrice = static_cast<int>(characterManager->GetCharacterPriceWithIndex(InCharacterIndex))
		; money >= characterPrice)
	{
		money -= characterPrice;
		characterBit |= RetCharacterBit;

		return -1;
	}

	// 돈 없나벼.
	return 0;
}

/*
	VipCodeProcess
	
		인자값 : void
		반환값 : -1 == 성공,
				  0 == VipCode이거 아님.
				  1 == 이미 받았던 적 잇음.
*/
_NODISCARD int UserData::VipCodeProcess(const wstring& InInputtedString)
{
	if (int compBit = static_cast<int>(characterManager->GetCharacterBitWithIndex(CharacterManager::CHARACTER_INDEX::VICTORY))
		; (achievementBit & compBit) == compBit)
	{
		return 1;
	}

	if (InInputtedString.compare(VIP_CODE) == 0)
	{
		// achievementBit는 지금 쫌 계륵
		achievementBit |= static_cast<int>(characterManager->GetCharacterBitWithIndex(CharacterManager::CHARACTER_INDEX::VICTORY));
		characterBit |= static_cast<int>(characterManager->GetCharacterBitWithIndex(CharacterManager::CHARACTER_INDEX::VICTORY));

		money += 100000000;	//	1억....부럽다...

		return -1;
	}

	return 0;
}