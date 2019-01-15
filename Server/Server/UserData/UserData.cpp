#include "../UserData/UserData.h"

#include "../IOCPServer/SocketInfo.h"

const string UserData::VIP_CODE = VIP_CODE_STRING;
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

	if ((itemBit & InItemIndex) == InItemIndex)
	{
		// �̹� ������ ������.
		return 1;
	}

	if (int itemPrice = itemManager->GetItemPriceWithIndex(InItemIndex)
		; money >= itemPrice)
	{
		money -= itemPrice;
		itemBit |= InItemIndex;

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

	if ((characterBit & InCharacterIndex) == InCharacterIndex)
	{
		// �̹� ������ ������.
		return 1;
	}

	if (int characterPrice = characterManager->GetCharacterPriceWithIndex(InCharacterIndex)
		; money >= characterPrice)
	{
		money -= characterPrice;
		characterBit |= InCharacterIndex;

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
_NODISCARD int UserData::VipCodeProcess(const string& InInputtedString)
{
	if ((achievementBit & 0x1) == 0x1)
	{
		return 1;
	}

	if (InInputtedString.compare(VIP_CODE) == 0)
	{
		achievementBit |= 0x1;
		characterBit |= 0x1;

		money += 100000000;

		return -1;
	}

	return 0;
}