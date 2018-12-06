#pragma once

#include "../PCH/stdafx.h"
#include "../IOCPServer/SocketInfo.h"

namespace CUSTOM_QUEUE
{
	template<typename DATA>
	class CustomNode
	{
		DATA data;

	public:
		CustomNode<DATA>* left;
		CustomNode<DATA>* right;

	public:
		CustomNode<DATA>(const DATA& InData)
			: data(InData), left(nullptr), right(nullptr)
		{}

		CustomNode<DATA>(const DATA& InData, CustomNode<DATA>* InLeft, CustomNode<DATA>* InRight)
			: data(InData), left(InLeft), right(InRight)
		{};

		~CustomNode<DATA>() = default;
	public:
		DATA& GetData()
		{
			return data;
		}
	};

	template<typename DATA>
	class CustomQueue
	{
		CustomNode<DATA>* pStart;
		CustomNode<DATA>* pEnd;

	public:
		CustomQueue<DATA>()
			: pStart(nullptr), pEnd(nullptr)
		{}

		CustomNode<DATA>& Push(const DATA& InDATA);

		DATA& Pop(bool& retBool);

		DATA& Pop();

	public:
		__inline bool IsEmpty() const
		{
			if (pStart == nullptr) return true;
			return false;
		}
	};

	template<typename DATA>
	CustomNode<DATA>& CustomQueue<DATA>::Push(const DATA& InDATA)
	{
		CustomNode<DATA> pRetNode = new CustomNode<DATA>(InDATA);

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

	template<typename DATA>
	DATA& CustomQueue<DATA>::Pop(bool& retBool)
	{
		if (pStart == nullptr)
		{
			retBool = false;
			return nullptr;
		}
		else
		{
			CustomNode<DATA> pRetNode = pStart;

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
			return pRetNode->GetData();
		}
	}

	template<typename DATA>
	DATA& CustomQueue<DATA>::Pop()
	{
		//if (pStart == nullptr)
		//{
		//	return nullptr;
		//}
		//else
		//{
		CustomNode<DATA> pRetNode = pStart;

		if (pStart->right == nullptr)
		{
			pStart = nullptr;
			pEnd = nullptr;
		}
		else
		{
			pStart = pStart->right;
		}
		return pRetNode->GetData();
		//}
	}
}