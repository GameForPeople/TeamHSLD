#include "../UserData/UserData.h"

#include "../IOCPServer/SocketInfo.h"

const wstring UserData::VIP_CODE = VIP_CODE_STRING;
unique_ptr<ItemManager> UserData::itemManager = make_unique<ItemManager>();
unique_ptr<CharacterManager> UserData::characterManager = make_unique<CharacterManager>();

/*
	BuyItem

	���ڰ� : enum ITEM_INDEX
	��ȯ�� : -1�̸� ����, 0�̸� ����� ����, 1�̸� �̹� �ִ� �������̿��� ����, 2�� �̻��Ѱ��ε�? ����.
*/
_NODISCARD int UserData::BuyItem(const int InItemIndex)
{
	if (InItemIndex > itemManager->GetItemCount())
	{
		// �׷� ������ ���Ⱦƿ�.
		return 2;
	}

	if (int retItemBit = static_cast<int>(itemManager->GetItemBitWithIndex(InItemIndex))
		; (itemBit & retItemBit) == retItemBit)
	{
		// �̹� ������ ������.
		return 1;
	}
	else if (int itemPrice = static_cast<int>(itemManager->GetItemPriceWithIndex(InItemIndex))
		; money >= itemPrice)
	{
		money -= itemPrice;
		itemBit |= retItemBit;

		return -1;
	}

	// �� ������.
	return 0;
}

/*
	BuyCharacter

	���ڰ� : enum ITEM_INDEX
	��ȯ�� : -1�̸� ����, 0�̸� ����� ����, 1�̸� �̹� �ִ� ĳ�����̿��� ����, 2�� �̻��Ѱ��ε�? ����.
*/
_NODISCARD int UserData::BuyCharacter(const int InCharacterIndex)
{
	if (InCharacterIndex > characterManager->GetCharacterCount())
	{
		// �׷� ĳ���� ���Ⱦƿ�.
		return 2;
	}

	if (int RetCharacterBit = static_cast<int>(characterManager->GetCharacterBitWithIndex[InCharacterIndex])
		; (characterBit & RetCharacterBit) == RetCharacterBit)
	{
		// �̹� ������ ������.
		return 1;
	}
	else if (int characterPrice = static_cast<int>(characterManager->GetCharacterPriceWithIndex(InCharacterIndex))
		; money >= characterPrice)
	{
		money -= characterPrice;
		characterBit |= RetCharacterBit;

		return -1;
	}

	// �� ������.
	return 0;
}

/*
	VipCodeProcess
	
		���ڰ� : void
		��ȯ�� : -1 == ����,
				  0 == VipCode�̰� �ƴ�.
				  1 == �̹� �޾Ҵ� �� ����.
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
		// achievementBit�� ���� �� �踤
		achievementBit |= static_cast<int>(characterManager->GetCharacterBitWithIndex(CharacterManager::CHARACTER_INDEX::VICTORY));
		characterBit |= static_cast<int>(characterManager->GetCharacterBitWithIndex(CharacterManager::CHARACTER_INDEX::VICTORY));

		money += 100000000;	//	1��....�η���...

		return -1;
	}

	return 0;
}