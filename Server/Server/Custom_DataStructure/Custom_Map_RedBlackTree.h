#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"

namespace CUSTOM_MAP
{

#pragma region [ Declare rbTreeNode, rbTree ]

	template <typename KEY, typename VALUE>
	class rbTree;

	template <typename KEY, typename VALUE>
	class rbTreeNode {
		friend class rbTree<KEY, VALUE>;

		// Define Color RED = true, BLACK = false;
		enum
		{
			RED = true,	//TRUE
			BLACK = false		//FALSE
		};

	private:
		KEY								key;	// Key
		VALUE							value;	// Value

		bool							color;	// false ( 0 ) == black , true (1) == red;

		rbTreeNode<KEY, VALUE>*			up;		// Parent Node Pointer
		rbTreeNode<KEY, VALUE>*			left;	// Left Child Node Pointer
		rbTreeNode<KEY, VALUE>*			right;	// Right Child Node Pointer

	public:
		// Define the constructor for pNullNode, buffer #1 Key, Value�� ���� �⺻�����ڰ� �ʿ��մϴ�.
		rbTreeNode<KEY, VALUE>() noexcept 
			: key(), value(), up(this), left(this), right(this), color(false)
		{}

		// Define the constructor for Insert 1
		rbTreeNode<KEY, VALUE>(const KEY& InKEY, const VALUE& InValue)
			: key(InKEY), value(InValue), up(nullptr), left(nullptr), right(nullptr), color(false)
		{}

		// Define the constructor for Insert 2
		rbTreeNode<KEY, VALUE>(const KEY& InKEY, const VALUE& InValue, rbTreeNode<KEY, VALUE>* pInNode, const bool& InColor)
			: key(InKEY), value(InValue), up(pInNode), left(pInNode), right(pInNode), color(InColor)
		{}

		// Define the destructor. 
		~rbTreeNode<KEY, VALUE>() = default;

	public:
		// Get Function
		__inline KEY					GetKey() const
		{
			return key;
		}

		__inline VALUE					GetValue() const
		{
			return value;
		}
		__inline VALUE&					SetValue()
		{
			return value;
		}

	public:
		//for Debug.
		void							PrintNodeInfo(rbTreeNode<KEY, VALUE>* pNullNode)
		{
			std::cout << "       ";

			cout << key << ' ';

			std::cout << "    ";

			if (color == RED)
				cout << "RED   ";
			else
				cout << "BLACK ";

			std::cout << "     ";

			cout << value << ' ';

			std::cout << "     ";

			if (up != pNullNode)
				cout << up->key << ' ';
			else
				cout << "NULL ";

			std::cout << "     ";

			if (left != pNullNode)
				cout << left->key << ' ';
			else
				cout << "NULL ";

			std::cout << "     ";

			if (right != pNullNode)
				cout << right->key << ' ';
			else
				cout << "NULL ";

			std::cout << endl;
		}
	};

	template <typename KEY, typename VALUE>
	class rbTree
	{
		// Define Color RED = true, BLACK = false;
		enum
		{
			RED = true,	//TRUE
			BLACK = false		//FALSE
		};

		rbTreeNode<KEY, VALUE>*				pNullNode;	// NullNode's Color is Black.
		rbTreeNode<KEY, VALUE>*				pRoot;		// Root!

	public:
		rbTree<KEY, VALUE>() noexcept
			: pNullNode(new rbTreeNode<KEY, VALUE>()), pRoot(pNullNode)
		{
			/* In Node Construct rbTreeNode::rbTreeNode()
			pRoot = pNullNode;
			pNullNode->up = 0;
			pNullNode->left = 0;
			pNullNode->right = 0;
			pNullNode->color = BLACK;
			*/
		};

		~rbTree<KEY, VALUE>()
		{
			_Clear();
			
			if (pNullNode == pRoot)
			{
				pRoot = nullptr;
			}

			delete pRoot;
			delete pNullNode;
		};

	public:
		rbTreeNode<KEY, VALUE>*				Search(const KEY& InKey, bool& RetResult) const;

		rbTreeNode<KEY, VALUE>*				Insert(const KEY& InKey, const VALUE& InValue);
		void								Delete(rbTreeNode<KEY, VALUE>* pDeletedNode);

	private:
		void								_ChangeForInsert(rbTreeNode<KEY, VALUE>* pInNode);
		void								_ChangeForDelete(rbTreeNode<KEY, VALUE>* pInNode);

		void								_LeftRotate(rbTreeNode<KEY, VALUE>* pInNode);
		void								_RightRotate(rbTreeNode<KEY, VALUE>* pInNode);

		__inline rbTreeNode<KEY, VALUE>*	_GetPrevNode(rbTreeNode<KEY, VALUE>* pInNode);
		__inline rbTreeNode<KEY, VALUE>*	_GetNextNode(rbTreeNode<KEY, VALUE>* pInNode);

		__inline rbTreeNode<KEY, VALUE>*	_GetSiblingNode(rbTreeNode<KEY, VALUE>* pInNode);
		__inline rbTreeNode<KEY, VALUE>*	_GetUncleNode(rbTreeNode<KEY, VALUE>* pInNode);

		void								_Clear();
		//for Debug
	public:
		_DEPRECATED void					PrintTree();

	private:
		_DEPRECATED void					_PrintNodes(rbTreeNode<KEY, VALUE>* pNodeBuffer);
	};

#pragma endregion

#pragma region [ Definition of Functions ]

	// ================================== Search


	/*
		Search(const KEY& InKey, bool& RetResult);
			- �־��� Ű ���� ������, Ʈ������ ��带 ã�� �Լ�.


		���� : Ž���ϴ� ����� Ű ��, Ž�� ���� ����

		��� : ��� ������(���ϰ�), Ž�� ���� ����(����)
	*/

	template <typename KEY, typename VALUE>
	rbTreeNode<KEY, VALUE>* rbTree<KEY, VALUE>::Search(const KEY& InKey, bool& RetResult) const
	{
		RetResult = false;

		rbTreeNode<KEY, VALUE>*  RetNodeBuffer = pRoot;

		while (true)
		{
			if (RetNodeBuffer == pNullNode)
			{
				RetResult = false;
				return RetNodeBuffer;
			}

			if (InKey == RetNodeBuffer->GetKey())
			{
				RetResult = true;
				return RetNodeBuffer;
			}

			if (InKey < RetNodeBuffer->GetKey())
				RetNodeBuffer = RetNodeBuffer->left;
			else
				RetNodeBuffer = RetNodeBuffer->right;
		}
	};


	// ================================== Insert

	/*
		Insert(const KEY& InKey, const VALUE& InValue);
			- ���ڷ� �����Ǵ� �ش� Ű, ��� ���� ������ ��带 �����ϴ� �Լ�.
			#1. ������ Ű���� ���Ͽ� Insert�� ��û�� ���, ������ ������ �� �� �ֽ��ϴ�.
			#2. ���ο��� Node�� ���� �Ҵ�(new) �� �Ͼ�ϴ�.

		���� : ����� Ű, ������

		��� : �ش� ��忡 ���� ������ ����.
	*/

	template <typename KEY, typename VALUE>
	rbTreeNode<KEY, VALUE>* rbTree<KEY, VALUE>::Insert(const KEY& InKey, const VALUE& InValue)
	{
#pragma region [ ������ ��� �Ҵ� & �ʿ��� ������ ���� ���� ]

		// ������ ����� �����͸� �����ϰ�, �Ҵ����ݴϴ�. ( Key, Value�� ���� ���� (Node���� ������ ���ǵ�)
		rbTreeNode<KEY, VALUE>* pRetNode = new rbTreeNode<KEY, VALUE>(InKey, InValue, pNullNode, RED);

		// �ʿ��� �����͵��� �����մϴ�.
		rbTreeNode<KEY, VALUE>* pTraversalNode = pRoot;			// ��ȸ������, ���� ����� ��ġ�� Ž���ϱ� ���� ��ȸ�ϴ� �������Դϴ�.
		rbTreeNode<KEY, VALUE>* pParentNode = pNullNode;		// �θ������, �θ� ��带 �����صα� ���� �������Դϴ�.

#pragma endregion

#pragma region [ �����ϴ� ����� �ڽĳ�� �� �� ���� ]

	// ������ ��� ��ó���Դϴ�. �ڽ� ��, ��� pNullNode�� �������ݴϴ�, ���� - ��Ʈ�� ��Ģ�� ����, ������ ����� ���� RED(TRUE)�� �������ݴϴ�.
	// �Ʒ� 3����, �����ڿ��� ó��.
	//pRetNode->left = pNullNode;
	//pRetNode->right = pNullNode;
	//pRetNode->color = RED;

#pragma endregion

#pragma region [ �����ϴ� ����� �θ��� ã�� ]
	// ���Ե� �� �ִ� �� �ڸ����� �̵��մϴ�.
	// pTraversalNode�� pRetNode�� ���Ե� �� �ִ� ��ġ(��Ȯ���� Ʈ���� ��������� �ڽ�)���� �̵��ϴ� ���-������ �Դϴ�.
	// pParentNode�� pNodeA�� ���� while�������� ��ġ( ���� pNodeA�� �θ� ��� ������) �Դϴ�.
		while (pTraversalNode != pNullNode)
		{
			//���� ��ġ�� pParentNode�� �����ϰ�, ���� ��ġ�� �̵��մϴ�.
			pParentNode = pTraversalNode;

			//Ű �񱳸� ����, ������ ����� ��ġ (Left Or Right)�� �����մϴ�. (Ʈ�� ���������� Ž��)
			if (pRetNode->GetKey() /* == InKey */ < pTraversalNode->GetKey())
				pTraversalNode = pTraversalNode->left;
			else
				pTraversalNode = pTraversalNode->right;
		}

		// �����ϴ� �������� up Point(�θ� ��� ��ġ)�� ����, pParentNode(���� ��� ������ - �θ� ��ġ)�� �־��ݴϴ�.
		pRetNode->up = pParentNode;
#pragma endregion

#pragma region [ �����ϴ� ����� ��ġ ���ϱ� ]

		// ���� �θ� ���(RetNode->up == oldBuffer)�� pNullNode�� ���, ������ ���� Ʈ���� pRoot��尡 �˴ϴ�.
		if (pParentNode == pNullNode)
		{
			pRoot = pRetNode;
		}
		else
		{
			// Ʈ���� ���� ��尡 �ƴ� ���, �θ� ���( pParentNode == pRetNode->up)�� Ű ���� ����, �θ� ��忡���� ��ġ (Left Or Right)�� ���� �� �������ݴϴ�.
			if (pRetNode->GetKey() < pParentNode->GetKey() /* ==  pRetNode->up->GetKey()	*/)
				pParentNode->left = pRetNode;
			else
				pParentNode->right = pRetNode;

			// ���� ����ߴ� ����Ʈ ����. ������ �߸����� ��� ����.
			pTraversalNode = pRetNode;

			// ���� ��, ���� ���θ� �˻��ϰ� ���� �ʿ� ó���� �ϴ� �Լ��Դϴ�. ( [Insert Case 1 ] ù �����϶��� �ʿ�����ϴ�.(���� if��, -- if (pParentNode == pNullNode) )
			_ChangeForInsert(pTraversalNode);
		}

#pragma endregion

		// ���� - ��Ʈ�� ��Ģ�� ����, � ��ȯ�� �Ͼ��, ���������� ������������ ��Ʈ ���� �׻� �������� �����ؾ��մϴ�.
		pRoot->color = BLACK;

		////(�����)���� �Ҷ�����, Ʈ���� ����մϴ�.
		//this->PrintTree();

		// ������ ��带 �������ݴϴ�.
		return pRetNode;
	};


	/*
		_ChangeForInsert(rbTreeNode<KEY, VALUE>* RetNode);
			- Insert �Լ� ���ο��� ���Ǹ�, ��带 Insert �� �Ŀ���, Red-Black Tree�� Ư¡�� �����ϱ� ���� �˻� �� ó���� ���ִ� �Լ��Դϴ�.

			#1. ���� �̷��� ��Ű ���, ����-�� Ʈ���� Ȯ���� �ּ���! https://ko.wikipedia.org/wiki/%EB%A0%88%EB%93%9C-%EB%B8%94%EB%9E%99_%ED%8A%B8%EB%A6%AC

		���� : Insert�� ����� ������

		��� : void
	*/

	template <typename KEY, typename VALUE>
	void rbTree<KEY, VALUE>::_ChangeForInsert(rbTreeNode<KEY, VALUE>* pRetNode)
	{
	LIKE_RECURSION:

		if (pRetNode->up->color == BLACK)
		{
			return;
		}

		rbTreeNode<KEY, VALUE>* pUncleNode = _GetUncleNode(pRetNode);
		rbTreeNode<KEY, VALUE>* pGrandNode = pRetNode->up->up;

		if (pUncleNode->color == RED /*&& pUncleNode != pNullNode // �� ������ �������� ����. */)
		{
			pRetNode->up->color = BLACK;
			pUncleNode->color = BLACK;
			pGrandNode->color = RED;


			// for goto
			pRetNode = pGrandNode;
			goto LIKE_RECURSION;
			//return;
		}

		if ((pRetNode == pRetNode->up->right) && (pRetNode->up == pGrandNode->left))
		{
			_LeftRotate(pRetNode->up);
			pRetNode = pRetNode->left;
		}
		else if ((pRetNode == pRetNode->up->left) && (pRetNode->up == pGrandNode->right)) {
			_RightRotate(pRetNode->up);
			pRetNode = pRetNode->right;
		}

		pGrandNode = pRetNode->up->up;

		pRetNode->up->color = BLACK;
		pGrandNode->color = RED;

		if (pRetNode == pRetNode->up->left)
			_RightRotate(pGrandNode);
		else
			_LeftRotate(pGrandNode);
	};


	// ================================== Delete

	/*
		Delete(rbTreeNode<KEY, VALUE>* DeletedNode);
			- ���ڷ� �����Ǵ� ����� �����͸� Ȱ����, �ش� ��带 �����մϴ�.

			#1. ���� ���� - ��Ʈ�� ��İ� ���� �ٸ� ���� Copy Value�� �ƴ϶�, Node ��ü�� �����ϴ� ���Դϴ�.
				( ���ο��� Node�� ���� ptr�� Ȱ���� ��, �̿� ���� ������ �����ϱ� ����.)

			!1. ���ο��� Node�� ���� �޸� ȸ��(delete) �� �Ͼ�ϴ�.

			?1. deleted�� ����� �����Ͱ� pNullNode�� ����Ű��, nullptr ���� ������ �������� ������?


		���� : �����Ϸ��� ����� ������

		��� : void
	*/

	template <typename KEY, typename VALUE>
	void rbTree<KEY, VALUE>::Delete(rbTreeNode<KEY, VALUE>* pDeletedNode)
	{
		if (pDeletedNode == pNullNode)
		{
			return;
		}

		rbTreeNode<KEY, VALUE>* pBuffer = pNullNode;
		rbTreeNode<KEY, VALUE>* pChildBuffer = pNullNode;
		bool deletedNodeColor = BLACK;

		if (pDeletedNode->left != pNullNode && pDeletedNode->right != pNullNode)
		{
			pBuffer = _GetNextNode(pDeletedNode);
			pChildBuffer = pBuffer->right;

			if (pDeletedNode->right == pBuffer)
			{
				if (pChildBuffer == pNullNode)
				{
					pChildBuffer->up = pBuffer;
				}
			}
			else if (pChildBuffer != pNullNode)
			{
				pChildBuffer->up = pBuffer->up;
				pBuffer->up->left = pChildBuffer;
			}
			else
			{
				// TEST ------------------------
				pChildBuffer->up = pBuffer->up;
				// TEST ------------------------

				pBuffer->up->left = pNullNode;
			}

			//pDeletedNode->key = pBuffer->key;
			//pDeletedNode->value = pBuffer->value;

			if (pDeletedNode->up == pNullNode)
			{
				pRoot = pBuffer;
			}
			else
			{
				if (pDeletedNode->up->left == pDeletedNode)
					pDeletedNode->up->left = pBuffer;
				else
					pDeletedNode->up->right = pBuffer;
			}

			pBuffer->up = pDeletedNode->up;

			pDeletedNode->left->up = pBuffer;
			pBuffer->left = pDeletedNode->left;

			if (pDeletedNode->right != pBuffer)
			{
				pDeletedNode->right->up = pBuffer;
				pBuffer->right = pDeletedNode->right;
			}

			deletedNodeColor = pBuffer->color;
			pBuffer->color = pDeletedNode->color;
		}
		else if (pDeletedNode->left != pNullNode)
		{
			pBuffer = pDeletedNode;
			pChildBuffer = pBuffer->left;

			pChildBuffer->up = pBuffer->up;

			if (pBuffer->up == pNullNode)
				pRoot = pChildBuffer;
			else
			{
				if (pBuffer->up->left == pBuffer)
					pBuffer->up->left = pChildBuffer;
				else
					pBuffer->up->right = pChildBuffer;
			}
			deletedNodeColor = pBuffer->color;
		}
		else if (pDeletedNode->right != pNullNode)
		{
			pBuffer = pDeletedNode;
			pChildBuffer = pBuffer->right;

			pChildBuffer->up = pBuffer->up;

			if (pBuffer->up == pNullNode)
				pRoot = pChildBuffer;
			else
			{
				if (pBuffer->up->left == pBuffer)
					pBuffer->up->left = pChildBuffer;
				else
					pBuffer->up->right = pChildBuffer;
			}

			deletedNodeColor = pBuffer->color;
		}
		else
		{
			pBuffer = pDeletedNode;

			// TEST ------------------------
			pChildBuffer->up = pBuffer->up;
			// TEST ------------------------

			if (pBuffer->up == pNullNode)
				pRoot = pNullNode;//pChildBuffer;
			else
			{
				if (pBuffer->up->left == pBuffer)
					pBuffer->up->left = pNullNode;
				else
					pBuffer->up->right = pNullNode;
			}

			deletedNodeColor = pBuffer->color;
		}

		if (deletedNodeColor == BLACK)
		{
			if (pChildBuffer->color == BLACK)
				_ChangeForDelete(pChildBuffer);
			else
				pChildBuffer->color = BLACK;
		}

		pRoot->color = BLACK;

		// TEST ------------------------
		pNullNode->up = nullptr;
		// TEST ------------------------

		//delete pBuffer;
		delete pDeletedNode;
	};


	/*
		_ChangeForDelete(rbTreeNode<KEY, VALUE>* pInNode);
			- Delete �Լ� ���ο��� ���Ǹ�, ��带 Delete �� �Ŀ���, Red-Black Tree�� Ư¡�� �����ϱ� ���� �˻� �� ó���� ���ִ� �Լ��Դϴ�.

			#1. ���� �̷��� ��Ű ���, ����-�� Ʈ���� Ȯ���� �ּ���! https://ko.wikipedia.org/wiki/%EB%A0%88%EB%93%9C-%EB%B8%94%EB%9E%99_%ED%8A%B8%EB%A6%AC

		���� : Delete�� ����� NextNode( Successor )�� ChildNode (dir - Right)

		��� : void
	*/

	template <typename KEY, typename VALUE>
	void rbTree<KEY, VALUE>::_ChangeForDelete(rbTreeNode<KEY, VALUE>* pInNode)
	{
	LIKE_RECURSION:

		if (pInNode->up == pNullNode)
			return;

		rbTreeNode<KEY, VALUE>* pSiblingNode = _GetSiblingNode(pInNode);

		if (pSiblingNode->color == RED)
		{
			pInNode->up->color = RED;
			pSiblingNode->color = BLACK;

			if (pInNode == pInNode->up->left)
				_LeftRotate(pInNode->up);
			else
				_RightRotate(pInNode->up);
		}

		pSiblingNode = _GetSiblingNode(pInNode);

		if (pInNode->up->color == BLACK && pSiblingNode->color == BLACK && pSiblingNode->left->color == BLACK && pSiblingNode->right->color == BLACK)
		{
			pSiblingNode->color = RED;
			pInNode = pInNode->up;

			// [Delete_Case_1] pInNode->up;
			goto LIKE_RECURSION;
		}

		else if (pInNode->up->color == RED && pSiblingNode->color == BLACK && pSiblingNode->left->color == BLACK && pSiblingNode->right->color == BLACK)
		{
			pSiblingNode->color = RED;
			pInNode->up->color = BLACK;
		}

		else {
			if (pSiblingNode->color == BLACK)
			{
				if (pInNode == pInNode->up->left && pSiblingNode->right->color == BLACK && pSiblingNode->left->color == RED)
				{
					pSiblingNode->color = RED;
					pSiblingNode->left->color = BLACK;
					_RightRotate(pSiblingNode);
				}
				else if (pInNode == pInNode->up->right && pSiblingNode->left->color == BLACK && pSiblingNode->right->color == RED)
				{
					pSiblingNode->color = RED;
					pSiblingNode->right->color = BLACK;
					_LeftRotate(pSiblingNode);
				}
			}

			pSiblingNode = _GetSiblingNode(pInNode);

			pSiblingNode->color = pInNode->up->color;
			pInNode->up->color = BLACK;

			if (pInNode == pInNode->up->left)
			{
				pSiblingNode->right->color = BLACK;
				_LeftRotate(pInNode->up);
			}
			else {
				pSiblingNode->left->color = BLACK;
				_RightRotate(pInNode->up);
			}
		}
	};



	// ================================== Rotate

	template <typename KEY, typename VALUE>
	void rbTree<KEY, VALUE>::_LeftRotate(rbTreeNode<KEY, VALUE>* pRetNode)
	{
		rbTreeNode<KEY, VALUE>* pParentNode = pRetNode->up;
		rbTreeNode<KEY, VALUE>* pRightChildNode = pRetNode->right;

		if (pParentNode == pNullNode)
			pRoot = pRightChildNode;
		else
		{
			if (pRetNode == pParentNode->left)
				pParentNode->left = pRightChildNode;

			else
				pParentNode->right = pRightChildNode;
		}

		pRightChildNode->up = pRetNode->up;

		pRetNode->right = pRightChildNode->left;
		pRightChildNode->left->up = pRetNode;

		pRightChildNode->left = pRetNode;
		pRetNode->up = pRightChildNode;
	};

	template <typename KEY, typename VALUE>
	void rbTree<KEY, VALUE>::_RightRotate(rbTreeNode<KEY, VALUE>* pRetNode)
	{
		rbTreeNode<KEY, VALUE>* pParentNode = pRetNode->up;
		rbTreeNode<KEY, VALUE>* pLeftChildNode = pRetNode->left;

		if (pParentNode == pNullNode)
			pRoot = pLeftChildNode;
		else
		{
			if (pRetNode == pRetNode->up->left)
				pRetNode->up->left = pLeftChildNode;
			else
				pParentNode->right = pLeftChildNode;
		}

		pLeftChildNode->up = pRetNode->up;

		pRetNode->left = pLeftChildNode->right;
		pLeftChildNode->right->up = pRetNode;

		pLeftChildNode->right = pRetNode;
		pRetNode->up = pLeftChildNode;
	};

	/*
	Clear();
	- pNullNode�� �����ϰ�, Ʈ���� �Ҵ�� ��� ������ �ݳ��մϴ�.
	���� : void
	��� : void
	*/
	template <typename KEY, typename VALUE>
	void rbTree<KEY, VALUE>::_Clear()
	{
		rbTreeNode<KEY, VALUE>* pTraversalNode = pRoot;
		rbTreeNode<KEY, VALUE>* pDeletedNode = pRoot;

		while (7)
		{
			if (pTraversalNode->left != pNullNode)
			{
				pTraversalNode = pTraversalNode->left;
				continue;
			}

			if (pTraversalNode->right != pNullNode)
			{
				pTraversalNode = pTraversalNode->right;
				continue;
			}

			pDeletedNode = pTraversalNode;

			if (pTraversalNode->up != pNullNode)
			{
				if (pTraversalNode->up->left == pTraversalNode)
					pTraversalNode->up->left = pNullNode;
				else /* if (pTraversalNode->up->right == pTraversalNode) */
					pTraversalNode->up->right = pNullNode;

				pTraversalNode = pTraversalNode->up;

				delete pDeletedNode;
			}
			else
			{
				delete pDeletedNode;
				break;
			}
		}

		pRoot = pNullNode;
	};

	// ================================== GetNode

	/*
		_GetPrevNode(rbTreeNode<KEY, VALUE>* InNode);

			Predecessor�� ���ϴ� �Լ�, ������ �ʽ��ϴ�.


		���� : Delete�Ǿ�, PrevNode�� ���ؾ��ϴ� ����� ������

		��� : Predecessor Node's Pointer
	*/

	template <typename KEY, typename VALUE>
	rbTreeNode<KEY, VALUE>*	rbTree<KEY, VALUE>::_GetPrevNode(rbTreeNode<KEY, VALUE>* pInNode)
	{
		rbTreeNode<KEY, VALUE>* pRetNode = pInNode;

		if (pRetNode->left != pNullNode) {

			pRetNode = pRetNode->left;

			while (pRetNode->right != pNullNode)
			{
				pRetNode = pRetNode->right;
			}

			return pRetNode;
		}
	};

	/*
		_GetNextNode(rbTreeNode<KEY, VALUE>* InNode);
		����(�����Դϴ�.) : https://www.geeksforgeeks.org/inorder-successor-in-binary-search-tree/

			- ���� �����帮��, ���� ��� ������ GetNextNode��� �Լ����� ������
			���� ��Ī�� Predecessor, Successor�̸� ����, �ش� ����� ���� ����Ʈ���� �ִ밪 (Predecessor), �ش� ����� ���� ����Ʈ���� �ּҰ� (Successor) �Դϴ�.

			�̸� ���ؾ��� ����,
			�����Ǵ� ��尡 �� ���� pNullNode�� �ƴ� �ڽ��� ���� ���, �ش� ����� �ڸ��� � ��尡 �;��ϴ� ���� ���� ��,
			�� �ڸ��� �� �� �ִ� ���� Predecessor �Ǵ� Successor�� �ֽ��ϴ�.

			//
			�̿� ������ �����ϴ� ��尡 �ڽ��� ���� ���, ��� �����ϸ� �ǰ�,
			�����ϴ� ��尡 1���� �ڽ��� ���� ���� ���, �ش� �ڽ��� �� �ڸ��� ��ġ��Ű�� �˴ϴ�(like list)


		���� : Delete�Ǿ�, NextNode�� ���ؾ��ϴ� ����� ������

		��� : Successor Node's Pointer
	*/

	template <typename KEY, typename VALUE>
	rbTreeNode<KEY, VALUE>*	rbTree<KEY, VALUE>::_GetNextNode(rbTreeNode<KEY, VALUE>* pInNode)
	{
		rbTreeNode<KEY, VALUE>* pRetNode = pInNode;

		if (pRetNode->right != pNullNode) {

			pRetNode = pRetNode->right;

			while (pRetNode->left != pNullNode)
			{
				pRetNode = pRetNode->left;
			}

			return pRetNode;
		}
	};


	/*
		_GetSiblingNode(rbTreeNode<KEY, VALUE>* pInNode);

			- pInNode�� ���� ����� �����͸� ���ϴ� �Լ��Դϴ�.


		���� : ����� ������

		��� : ���� ����� ������
	*/

	template <typename KEY, typename VALUE>
	rbTreeNode<KEY, VALUE>*	rbTree<KEY, VALUE>::_GetSiblingNode(rbTreeNode<KEY, VALUE>* pInNode)
	{
		rbTreeNode<KEY, VALUE>* pBufferNode = pInNode->up;

		if (pBufferNode->left == pInNode)
		{
			return pBufferNode->right;
		}
		else //if (pBufferNode->right == pInNode)
		{
			return pBufferNode->left;
		}
		//else
		//{
		//	std::cout << "Error - ���� ��带 ��ȯ�ϴ� �Լ����� ���ܰ� �߻��߽��ϴ�. \n";
		//}
	}

	/*
		_GetUncleNode(rbTreeNode<KEY, VALUE>* pInNode);

			- pInNode�� Uncle ����� �����͸� ���ϴ� �Լ��Դϴ�.


		���� : ����� ������

		��� : ���� ����� ������
	*/

	template <typename KEY, typename VALUE>
	rbTreeNode<KEY, VALUE>*	rbTree<KEY, VALUE>::_GetUncleNode(rbTreeNode<KEY, VALUE>* pInNode)
	{
		rbTreeNode<KEY, VALUE>* pGrandParentNode = pInNode->up->up;

		if (pInNode->up == pGrandParentNode->left)
			return pGrandParentNode->right;
		else
			return pGrandParentNode->left;
		//else
		//{
		//		std::cout << "Error - Uncle ��带 ��ȯ�ϴ� �Լ����� ���ܰ� �߻��߽��ϴ�. \n";
		//}
	}



	// ================================== Debug Function


	/*
		!1. ���ο��� ����Լ��� ����ϰ� �ֽ��ϴ�. Ʈ�� ���� ����� ������ ���� ���, ���� �����÷ο찡 �߻��� �� �ֽ��ϴ�.
			(����� �뵵�θ� ����ϴ� ���� ��õ�帳�ϴ�.)
	*/

	template <typename KEY, typename VALUE>
	_DEPRECATED void rbTree<KEY, VALUE>::PrintTree()
	{
		rbTreeNode<KEY, VALUE>* pNodeBuffer = pRoot;

		if (pRoot != pNullNode)
		{
			std::cout << "pRootNode's Key : " << pRoot->GetKey() << "\n";
			std::cout << "Node - Key   Color   Value    Up     Left     Right \n";
			_PrintNodes(pRoot);
		}
		else
			cout << "Tree pRoot is pNullNode (VOID-TREE) \n";
	}

	template <typename KEY, typename VALUE>
	_DEPRECATED void rbTree<KEY, VALUE>::_PrintNodes(rbTreeNode<KEY, VALUE>* pNodeBuffer)
	{
		if (pNodeBuffer->left != pNullNode)
			_PrintNodes(pNodeBuffer->left);

		pNodeBuffer->PrintNodeInfo(pNullNode);

		if (pNodeBuffer->right != pNullNode)
			_PrintNodes(pNodeBuffer->right);
	}

#pragma endregion

}