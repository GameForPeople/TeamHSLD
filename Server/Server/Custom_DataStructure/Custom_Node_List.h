#pragma once

#include "../PCH/stdafx.h"
#include "../GameRoom/GameRoom.h"

namespace CUSTOM_LIST
{
	template <typename NODE, typename NODE_NEED>
	class CustomList
	{
		NODE* pRoot;

	public:
		CustomList<NODE, NODE_NEED>()
		{
			int ibuffer{ 0 };

			//for Cap - Dummy
			pRoot = new NODE(ibuffer);
			//WaitNodeCont = nullptr;
		}

	public:
		NODE* Create(const NODE_NEED& pInUserNode);

		void Push(NODE* pRetNode);

		void Pop(NODE* pRetNode);

		NODE* GetRootNode(NODE* pRetNode);

		__inline bool IsEmpty() const
		{
			if (pRoot->right == nullptr)
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

	template <typename NODE, typename NODE_NEED>
	NODE* CustomList<NODE, NODE_NEED>::Create(const NODE_NEED& pInUserNode)
	{
		NODE* pRetNode = new NODE(pInUserNode, nullptr, nullptr);
		pRoot->left = pRetNode;
		pRetNode->right = pRoot;
		pRoot = pRetNode;

		return pRetNode;
	}

	template <typename NODE, typename NODE_NEED>
	void CustomList<NODE, NODE_NEED>::Push(NODE* pRetNode)
	{
		// �����͸� �������ϰ� ��.
		pRoot->left = pRetNode;
		pRetNode->right = pRoot;
		pRetNode->left = nullptr;
		pRoot = pRetNode;
	}

	template <typename NODE, typename NODE_NEED>
	void CustomList<NODE, NODE_NEED>::Pop(NODE* pRetNode)
	{
		if (pRoot == nullptr)
			return;

		// ��� ��带 �����Ҷ�.
		if (pRetNode->left == nullptr)
		{
			pRoot = pRoot->right;
			pRoot->left = nullptr;
			return;
		}

		pRetNode->left->right = pRetNode->right;
		pRetNode->right->left = pRetNode->left;
	}

	template <typename NODE, typename NODE_NEED>
	NODE* CustomList<NODE, NODE_NEED>::GetRootNode(NODE* pRetNode)
	{
		pRetNode = pRoot;

		pRoot = pRoot->right;
		//pRetNode->right = nullptr; // ���� �ʿ� ���� ������ ����.

		pRoot->left = nullptr;

		return pRetNode;
	}
}