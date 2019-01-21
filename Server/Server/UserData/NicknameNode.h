#pragma once

#include "../PCH/stdafx.h"

/*
	SERVER_DEV78 

	닉네임 트리의 기능 확대를 위해 기존 Nickname - ID 구조에서, Nickname-NicknameNode구조로 변경.
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