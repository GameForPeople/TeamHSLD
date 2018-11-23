#pragma once

#include "../PCH/stdafx.h"
#include "../GameRoom/GameRoom.h"

class room_list
{
	GameRoom* pDoor;

public:
	room_list()
	{
		int ibuffer{ 0 };

		//for Cap - Dummy
		pDoor = new GameRoom(ibuffer);
		//WaitNodeCont = nullptr;
	}

	GameRoom* Create(rbTreeNode<string, UserData>* pInUserNode)
	{
		GameRoom* pRetNode = new GameRoom(pInUserNode,nullptr, nullptr);
		pDoor->left = pRetNode;
		pRetNode->right = pDoor;
		pDoor = pRetNode;

		return pRetNode;
	}

	void Push(GameRoom* pRetNode)
	{
		// �����͸� �������ϰ� ��.
		pDoor->left = pRetNode;
		pRetNode->right = pDoor;
		pRetNode->left = nullptr;
		pDoor = pRetNode;
	}

	void Pop(GameRoom* pRetNode)
	{
		if (pDoor == nullptr)
			return;

		// ��� ��带 �����Ҷ�.
		if (pRetNode->left == nullptr)
		{
			pDoor = pDoor->right;
			pDoor->left = nullptr;
			return;
		}

		pRetNode->left->right = pRetNode->right;
		pRetNode->right->left = pRetNode->left;
	}

	GameRoom* GetOneRoom(GameRoom* pRetNode)
	{
		pRetNode = pDoor;

		pDoor = pDoor->right;
		//pRetNode->right = nullptr; // ���� �ʿ� ���� ������ ����.

		pDoor->left = nullptr;

		return pRetNode;
	}

	__inline bool IsEmpty() const
	{
		if (pDoor->right == nullptr)
			return true;
		
		return false;
	}

	void Print()
	{
		//GameRoom* newNode = WaitNodeCont;
		//
		//while (newNode->right != nullptr)
		//{
		//	newNode->Print();
		//	newNode = newNode->right;
		//}
	}
};