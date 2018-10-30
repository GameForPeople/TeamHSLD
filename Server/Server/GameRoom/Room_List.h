#pragma once

#include "../PCH/stdafx.h"
#include "../GameRoom/NewGameRoom.h"

class room_list
{
	NewGameRoom* pDoor;

public:
	room_list()
	{
		int ibuffer{ 0 };

		//for Cap - Dummy
		pDoor = new NewGameRoom(ibuffer);
		//WaitNodeCont = nullptr;
	}

	void Create(NewGameRoom* pRetNode)
	{
		pDoor->left = pRetNode;
		pRetNode->right = pDoor;
		pDoor = pRetNode;
	}

	void Push(NewGameRoom* pRetNode)
	{
		// �����͸� �������ϰ� ��.
		pDoor->left = pRetNode;
		pRetNode->right = pDoor;
		pRetNode->left = nullptr;
		pDoor = pRetNode;
	}

	void Pop(NewGameRoom* pRetNode)
	{
		// ��� ��带 �����Ҷ�.
		if (pRetNode->left == nullptr)
		{
			pDoor = pDoor->right;
			pDoor->left = nullptr;
		}
		pRetNode->left->right = pRetNode->right;
		pRetNode->right->left = pRetNode->left;
	}

	void GetOneRoom(NewGameRoom* pRetNode)
	{
		pRetNode = pDoor;

		pDoor = pDoor->right;
		pRetNode->right = nullptr;

		pDoor->left = nullptr;
	}

	void Print()
	{
		//NewGameRoom* newNode = WaitNodeCont;
		//
		//while (newNode->right != nullptr)
		//{
		//	newNode->Print();
		//	newNode = newNode->right;
		//}
	}
};