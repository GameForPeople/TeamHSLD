#pragma once

#include "../PCH/stdafx.h"

/*
	SERVER_DEV78 

	�г��� Ʈ���� ��� Ȯ�븦 ���� ���� Nickname - ID ��������, Nickname-NicknameNode������ ����.
*/
class NicknameNode
{
	Type_ID		id;
	BYTE		activeCharacterIndex;

public:
	NicknameNode(Type_ID InID, BYTE InCharacterIndex) noexcept
		: id(InID), activeCharacterIndex(InCharacterIndex)
	{};

	_NODISCARD __inline Type_ID&	GetID() noexcept { return id; };
	_NODISCARD __inline BYTE		GetActiveCharacterIndex() noexcept { return activeCharacterIndex; };

	__inline void SetID(Type_ID& InID) noexcept { id = InID; };
	__inline void SetActiveCharacterIndex(BYTE InCharacterIndex) noexcept { activeCharacterIndex = InCharacterIndex; };
};