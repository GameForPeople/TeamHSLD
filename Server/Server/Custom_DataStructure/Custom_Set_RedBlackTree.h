#pragma once

#include "../PCH/stdafx.h"

// add Custom
#include "../IOCPServer/UDPManager.h"

class UDPManager;

namespace CUSTOM_SET
{
#pragma region [ Declare rbTreeNode, rbTree ]

	template < typename DATA, typename KEY_TYPE>
	class rbTree;

	template <typename DATA, typename KEY_TYPE>
	class rbTreeNode {
		friend class rbTree<DATA, KEY_TYPE>;

		// Define Color RED = true, BLACK = false;
		enum
		{
			RED = true,			//TRUE
			BLACK = false		//FALSE
		};

	private:
		DATA							data;	// data

		bool							color;	// false ( 0 ) == black , true (1) == red;

		rbTreeNode<DATA, KEY_TYPE>*			up;		// Parent Node Pointer
		rbTreeNode<DATA, KEY_TYPE>*			left;	// Left Child Node Pointer
		rbTreeNode<DATA, KEY_TYPE>*			right;	// Right Child Node Pointer

	public:
		// Define the constructor for pNullNode, buffer #1 Key, Value�� ���� �⺻�����ڰ� �ʿ��մϴ�.
		rbTreeNode<DATA, KEY_TYPE>() noexcept
			: data(), up(this), left(this), right(this), color(BLACK)
		{}

		// Define the constructor for Insert 1
		rbTreeNode<DATA, KEY_TYPE>(const DATA& InDATA)
			: data(InDATA), up(nullptr), left(nullptr), right(nullptr), color(BLACK)
		{}

		// Define the constructor for Insert 2
		rbTreeNode<DATA, KEY_TYPE>(const DATA& InDATA, rbTreeNode<DATA, KEY_TYPE>* pInNode, const bool& InColor)
			: data(InDATA), up(pInNode), left(pInNode), right(pInNode), color(InColor)
		{}

		// Define the destructor. 
		~rbTreeNode<DATA, KEY_TYPE>() = default;

	public:
		// Get Function
		__inline DATA GetData() const
		{
			return data;
		}

		__inline KEY_TYPE GetKey() const
		{
			return data->GetKey();
		}

	public:
		//for Debug.
		void PrintNodeInfo(rbTreeNode<DATA, KEY_TYPE>* pNullNode)
		{
			std::cout << "       ";

			cout << GetKey() << ' ';

			std::cout << "    ";

			if (color == RED)
				cout << "RED   ";
			else
				cout << "BLACK ";

			std::cout << "     ";

			if (up != pNullNode)
				cout << up->data << ' ';
			else
				cout << "NULL ";

			std::cout << "     ";

			if (left != pNullNode)
				cout << left->data << ' ';
			else
				cout << "NULL ";

			std::cout << "     ";

			if (right != pNullNode)
				cout << right->data << ' ';
			else
				cout << "NULL ";

			std::cout << endl;
		}
	};

	template <typename DATA, typename KEY_TYPE>
	class rbTree
	{
		// Define Color RED = true, BLACK = false;
		enum
		{
			RED = true,			//TRUE
			BLACK = false		//FALSE
		};

		rbTreeNode<DATA, KEY_TYPE>*				pNullNode;	// NullNode's Color is Black.
		rbTreeNode<DATA, KEY_TYPE>*				pRoot;		// Root!

	public:
		rbTree<DATA, KEY_TYPE>() noexcept
			: pNullNode(new rbTreeNode<DATA, KEY_TYPE>()), pRoot(pNullNode)
		{
			/* In Node Construct rbTreeNode::rbTreeNode()
			pRoot = pNullNode;
			pNullNode->up = 0;
			pNullNode->left = 0;
			pNullNode->right = 0;
			pNullNode->color = BLACK;
			*/
		};

		~rbTree<DATA, KEY_TYPE>()
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
		DATA										Search(const KEY_TYPE& InKey, bool& RetResult) const;
		/*DATA&*/ void								Insert(const DATA& InDATA);
		void										Delete(rbTreeNode<DATA, KEY_TYPE>* pDeletedNode);
		void										DeleteWithSearch(const KEY_TYPE& InKey);

	private:
		void										_ChangeForInsert(rbTreeNode<DATA, KEY_TYPE>* pInNode);
		void										_ChangeForDelete(rbTreeNode<DATA, KEY_TYPE>* pInNode);

		void										_LeftRotate(rbTreeNode<DATA, KEY_TYPE>* pInNode);
		void										_RightRotate(rbTreeNode<DATA, KEY_TYPE>* pInNode);

		__inline rbTreeNode<DATA, KEY_TYPE>*		_GetPrevNode(rbTreeNode<DATA, KEY_TYPE>* pInNode);
		__inline rbTreeNode<DATA, KEY_TYPE>*		_GetNextNode(rbTreeNode<DATA, KEY_TYPE>* pInNode);

		__inline rbTreeNode<DATA, KEY_TYPE>*		_GetSiblingNode(rbTreeNode<DATA, KEY_TYPE>* pInNode);
		__inline rbTreeNode<DATA, KEY_TYPE>*		_GetUncleNode(rbTreeNode<DATA, KEY_TYPE>* pInNode);

		void										_Clear();		
		
	public: // for Project Function
		void TraversalForAnnouncement(UDPManager* pInUDPManager, rbTreeNode<DATA, KEY_TYPE>* pNodeBuffer)
		{
			if (pNodeBuffer != pNullNode)
			{
				TraversalForAnnouncement(pInUDPManager, pNodeBuffer->left);
				TraversalForAnnouncement(pInUDPManager, pNodeBuffer->right);
				//pInUDPManager->_SendAnnouncement(pNodeBuffer->GetData());
			}
		};

		__inline rbTreeNode<DATA, KEY_TYPE>* GetRootNode() const { return pRoot; };
		
	public: //for Debug
		_DEPRECATED void							PrintTree();

	private:
		_DEPRECATED void							_PrintNodes(rbTreeNode<DATA, KEY_TYPE>* pNodeBuffer);
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

	template <typename DATA, typename KEY_TYPE>
	DATA rbTree<DATA, KEY_TYPE>::Search(const KEY_TYPE& InKey, bool& RetResult) const
	{
		RetResult = false;

		rbTreeNode<DATA, KEY_TYPE>* RetNodeBuffer = pRoot;

		while (true)
		{
			if (RetNodeBuffer == pNullNode)
			{
				RetResult = false;
				return RetNodeBuffer->GetData();
			}

			if (InKey == RetNodeBuffer->GetKey())
			{
				RetResult = true;
				return RetNodeBuffer->GetData();
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

	template <typename DATA, typename KEY_TYPE>
	/*DATA&*/ void rbTree<DATA, KEY_TYPE>::Insert(const DATA& InDATA)
	{
#pragma region [ ������ ��� �Ҵ� & �ʿ��� ������ ���� ���� ]

		// ������ ����� �����͸� �����ϰ�, �Ҵ����ݴϴ�. ( Key, Value�� ���� ���� (Node���� ������ ���ǵ�)
		rbTreeNode<DATA, KEY_TYPE>* pRetNode = new rbTreeNode<DATA, KEY_TYPE>(InDATA, pNullNode, RED);

		// �ʿ��� �����͵��� �����մϴ�.
		rbTreeNode<DATA, KEY_TYPE>* pTraversalNode = pRoot;			// ��ȸ������, ���� ����� ��ġ�� Ž���ϱ� ���� ��ȸ�ϴ� �������Դϴ�.
		rbTreeNode<DATA, KEY_TYPE>* pParentNode = pNullNode;		// �θ������, �θ� ��带 �����صα� ���� �������Դϴ�.

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
		//return pRetNode->GetData();
	};


	/*
	_ChangeForInsert(rbTreeNode<KEY, VALUE>* RetNode);
	- Insert �Լ� ���ο��� ���Ǹ�, ��带 Insert �� �Ŀ���, Red-Black Tree�� Ư¡�� �����ϱ� ���� �˻� �� ó���� ���ִ� �Լ��Դϴ�.

	#1. ���� �̷��� ��Ű ���, ����-�� Ʈ���� Ȯ���� �ּ���! https://ko.wikipedia.org/wiki/%EB%A0%88%EB%93%9C-%EB%B8%94%EB%9E%99_%ED%8A%B8%EB%A6%AC

	���� : Insert�� ����� ������

	��� : void
	*/

	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::_ChangeForInsert(rbTreeNode<DATA, KEY_TYPE>* pRetNode)
	{
	LIKE_RECURSION:

		if (pRetNode->up->color == BLACK)
		{
			return;
		}

		rbTreeNode<DATA, KEY_TYPE>* pUncleNode = _GetUncleNode(pRetNode);
		rbTreeNode<DATA, KEY_TYPE>* pGrandNode = pRetNode->up->up;

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

	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::Delete(rbTreeNode<DATA, KEY_TYPE>* pDeletedNode)
	{
		if (pDeletedNode == pNullNode)
		{
			return;
		}

		rbTreeNode<DATA, KEY_TYPE>* pBuffer = pNullNode;
		rbTreeNode<DATA, KEY_TYPE>* pChildBuffer = pNullNode;
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


	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::DeleteWithSearch(const KEY_TYPE& InKey)
	{
		//--- Search
		bool isTrueSearch = false;

		rbTreeNode<DATA, KEY_TYPE>* RetNodeBuffer = pRoot;

		while (true)
		{
			if (RetNodeBuffer == pNullNode)
			{
				isTrueSearch = false;
				break;
			}

			if (InKey == RetNodeBuffer->GetKey())
			{
				isTrueSearch = true;
				break;
			}

			if (InKey < RetNodeBuffer->GetKey())
				RetNodeBuffer = RetNodeBuffer->left;
			else
				RetNodeBuffer = RetNodeBuffer->right;
		}
		//---

		if (!isTrueSearch)
			return;

		Delete(RetNodeBuffer);
	}

	/*
	_ChangeForDelete(rbTreeNode<KEY, VALUE>* pInNode);
	- Delete �Լ� ���ο��� ���Ǹ�, ��带 Delete �� �Ŀ���, Red-Black Tree�� Ư¡�� �����ϱ� ���� �˻� �� ó���� ���ִ� �Լ��Դϴ�.

	#1. ���� �̷��� ��Ű ���, ����-�� Ʈ���� Ȯ���� �ּ���! https://ko.wikipedia.org/wiki/%EB%A0%88%EB%93%9C-%EB%B8%94%EB%9E%99_%ED%8A%B8%EB%A6%AC

	���� : Delete�� ����� NextNode( Successor )�� ChildNode (dir - Right)

	��� : void
	*/

	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::_ChangeForDelete(rbTreeNode<DATA, KEY_TYPE>* pInNode)
	{
	LIKE_RECURSION:

		if (pInNode->up == pNullNode)
			return;

		rbTreeNode<DATA, KEY_TYPE>* pSiblingNode = _GetSiblingNode(pInNode);

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

	/*
		Clear();
			- pNullNode�� �����ϰ�, Ʈ���� �Ҵ�� ��� ������ �ݳ��մϴ�.
		
		���� : void
		��� : void
	*/
	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::_Clear()
	{
		rbTreeNode<DATA, KEY_TYPE>* pTraversalNode = pRoot;
		rbTreeNode<DATA, KEY_TYPE>* pDeletedNode = pRoot;

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


	// ================================== Rotate

	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::_LeftRotate(rbTreeNode<DATA, KEY_TYPE>* pRetNode)
	{
		rbTreeNode<DATA, KEY_TYPE>* pParentNode = pRetNode->up;
		rbTreeNode<DATA, KEY_TYPE>* pRightChildNode = pRetNode->right;

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

	template <typename DATA, typename KEY_TYPE>
	void rbTree<DATA, KEY_TYPE>::_RightRotate(rbTreeNode<DATA, KEY_TYPE>* pRetNode)
	{
		rbTreeNode<DATA, KEY_TYPE>* pParentNode = pRetNode->up;
		rbTreeNode<DATA, KEY_TYPE>* pLeftChildNode = pRetNode->left;

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


	// ================================== GetNode

	/*
	_GetPrevNode(rbTreeNode<KEY, VALUE>* InNode);

	Predecessor�� ���ϴ� �Լ�, ������ �ʽ��ϴ�.


	���� : Delete�Ǿ�, PrevNode�� ���ؾ��ϴ� ����� ������

	��� : Predecessor Node's Pointer
	*/

	template <typename DATA, typename KEY_TYPE>
	rbTreeNode<DATA, KEY_TYPE>*	rbTree<DATA, KEY_TYPE>::_GetPrevNode(rbTreeNode<DATA, KEY_TYPE>* pInNode)
	{
		rbTreeNode<DATA, KEY_TYPE>* pRetNode = pInNode;

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

	template <typename DATA, typename KEY_TYPE>
	rbTreeNode<DATA, KEY_TYPE>*	rbTree<DATA, KEY_TYPE>::_GetNextNode(rbTreeNode<DATA, KEY_TYPE>* pInNode)
	{
		rbTreeNode<DATA, KEY_TYPE>* pRetNode = pInNode;

		if (pRetNode->right != pNullNode) {

			pRetNode = pRetNode->right;

			while (pRetNode->left != pNullNode)
			{
				pRetNode = pRetNode->left;
			}

			return pRetNode;
		}
		std::cout << "ERROR RBTree - 773 \n";
	};


	/*
	_GetSiblingNode(rbTreeNode<KEY, VALUE>* pInNode);

	- pInNode�� ���� ����� �����͸� ���ϴ� �Լ��Դϴ�.


	���� : ����� ������

	��� : ���� ����� ������
	*/

	template <typename DATA, typename KEY_TYPE>
	rbTreeNode<DATA, KEY_TYPE>*	rbTree<DATA, KEY_TYPE>::_GetSiblingNode(rbTreeNode<DATA, KEY_TYPE>* pInNode)
	{
		rbTreeNode<DATA, KEY_TYPE>* pBufferNode = pInNode->up;

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

	template <typename DATA, typename KEY_TYPE>
	rbTreeNode<DATA, KEY_TYPE>*	rbTree<DATA, KEY_TYPE>::_GetUncleNode(rbTreeNode<DATA, KEY_TYPE>* pInNode)
	{
		rbTreeNode<DATA, KEY_TYPE>* pGrandParentNode = pInNode->up->up;

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

	template <typename DATA, typename KEY_TYPE>
	_DEPRECATED void rbTree<DATA, KEY_TYPE>::PrintTree()
	{
		rbTreeNode<DATA, KEY_TYPE>* pNodeBuffer = pRoot;

		if (pRoot != pNullNode)
		{
			std::cout << "pRootNode's Key : " << pRoot->GetKey() << "\n";
			std::cout << "Node - Key   Color   Value    Up     Left     Right \n";
			_PrintNodes(pRoot);
		}
		else
			cout << "Tree pRoot is pNullNode (VOID-TREE) \n";
	}

	template <typename DATA, typename KEY_TYPE>
	_DEPRECATED void rbTree<DATA, KEY_TYPE>::_PrintNodes(rbTreeNode<DATA, KEY_TYPE>* pNodeBuffer)
	{
		if (pNodeBuffer->left != pNullNode)
			_PrintNodes(pNodeBuffer->left);

		pNodeBuffer->PrintNodeInfo(pNullNode);

		if (pNodeBuffer->right != pNullNode)
			_PrintNodes(pNodeBuffer->right);
	}

#pragma endregion

}
