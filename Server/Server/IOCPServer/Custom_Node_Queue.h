#pragma once

#include "../PCH/stdafx.h"
#include "../IOCPServer/SocketInfo.h"

class CustomNode
{
	SocketInfo* data;

public:
	CustomNode * left;
	CustomNode * right;

public:
	CustomNode(SocketInfo* InData)
		: data(InData), left(nullptr), right(nullptr)
	{}

	CustomNode(SocketInfo* InData, CustomNode* InLeft, CustomNode* InRight)
		: data(InData), left(InLeft), right(InRight)
	{};

	~CustomNode() = default;
public:
	SocketInfo* GetData()
	{
		return data;
	}

	SOCKET& GetSocket()
	{
		return (*data).sock;
	}
};

class Custom_Node_Queue
{
	CustomNode* pStart;
	CustomNode* pEnd;

public:
	Custom_Node_Queue() 
		: pStart(nullptr), pEnd(nullptr)
	{}

	CustomNode* Push(SocketInfo* InDATA)
	{
		CustomNode* pRetNode = new CustomNode(InDATA);

		if (pEnd == nullptr)
		{
			pStart = pRetNode;
			pEnd = pRetNode;
		}
		else
		{
			pEnd->right = pRetNode;
			pRetNode->left = pEnd;

			pEnd = pRetNode;
		}

		return pRetNode;
	}

	CustomNode* Pop(bool& retBool)
	{
		if (pStart == nullptr)
		{
			retBool = false;
			return nullptr;
		}
		else
		{
			CustomNode* pRetNode = pStart;

			if (pStart->right == nullptr)
			{
				pStart = nullptr;
				pEnd = nullptr;
			}
			else
			{
				pStart = pStart->right;
			}

			retBool = true;
			return pRetNode;
		}
	}

	CustomNode* Pop()
	{
		//if (pStart == nullptr)
		//{
		//	return nullptr;
		//}
		//else
		//{
			CustomNode* pRetNode = pStart;

			if (pStart->right == nullptr)
			{
				pStart = nullptr;
				pEnd = nullptr;
			}
			else
			{
				pStart = pStart->right;
			}
			return pRetNode;
		//}
	}

public:
	__inline bool IsEmpty() const
	{
		if (pStart == nullptr) return true;
		return false;
	}
};
