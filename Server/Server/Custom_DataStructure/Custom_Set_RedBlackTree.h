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
		// Define the constructor for pNullNode, buffer #1 Key, Value에 대한 기본생성자가 필요합니다.
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
	- 주어진 키 값을 가지고, 트리에서 노드를 찾는 함수.


	인자 : 탐색하는 노드의 키 값, 탐색 성공 여부

	출력 : 노드 포인터(리턴값), 탐색 성공 여부(인자)
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
	- 인자로 제공되는 해당 키, 결과 값을 가지는 노드를 삽입하는 함수.
	#1. 동일한 키값에 대하여 Insert를 요청할 경우, 오류의 원인이 될 수 있습니다.
	#2. 내부에서 Node에 대한 할당(new) 가 일어납니다.

	인자 : 노드의 키, 데이터

	출력 : 해당 노드에 대한 포인터 제공.
	*/

	template <typename DATA, typename KEY_TYPE>
	/*DATA&*/ void rbTree<DATA, KEY_TYPE>::Insert(const DATA& InDATA)
	{
#pragma region [ 삽입할 노드 할당 & 필요한 포인터 변수 정의 ]

		// 리턴할 노드의 포인터를 선언하고, 할당해줍니다. ( Key, Value는 인자 전달 (Node에서 생성자 정의됨)
		rbTreeNode<DATA, KEY_TYPE>* pRetNode = new rbTreeNode<DATA, KEY_TYPE>(InDATA, pNullNode, RED);

		// 필요한 포인터들을 생성합니다.
		rbTreeNode<DATA, KEY_TYPE>* pTraversalNode = pRoot;			// 순회노드버퍼, 리턴 노드의 위치를 탐색하기 위해 순회하는 포인터입니다.
		rbTreeNode<DATA, KEY_TYPE>* pParentNode = pNullNode;		// 부모노드버퍼, 부모 노드를 저장해두기 위한 포인터입니다.

#pragma endregion

#pragma region [ 삽입하는 노드의 자식노드 및 색 설정 ]

															// 삽입한 노드 후처리입니다. 자식 좌, 우는 pNullNode로 설정해줍니다, 레드 - 블랙트리 법칙에 의해, 삽입한 노드의 색은 RED(TRUE)로 설정해줍니다.
															// 아래 3줄은, 생성자에서 처리.
															//pRetNode->left = pNullNode;
															//pRetNode->right = pNullNode;
															//pRetNode->color = RED;

#pragma endregion

#pragma region [ 삽입하는 노드의 부모노드 찾기 ]
															// 삽입될 수 있는 빈 자리까지 이동합니다.
															// pTraversalNode는 pRetNode가 삽입될 수 있는 위치(정확히는 트리의 리프노드의 자식)까지 이동하는 노드-포인터 입니다.
															// pParentNode는 pNodeA의 이전 while문에서의 위치( 현재 pNodeA의 부모 노드 포인터) 입니다.
		while (pTraversalNode != pNullNode)
		{
			//현재 위치를 pParentNode에 저장하고, 다음 위치로 이동합니다.
			pParentNode = pTraversalNode;

			//키 비교를 통해, 적합한 노드의 위치 (Left Or Right)를 결정합니다. (트리 구조에서의 탐색)
			if (pRetNode->GetKey() /* == InKey */ < pTraversalNode->GetKey())
				pTraversalNode = pTraversalNode->left;
			else
				pTraversalNode = pTraversalNode->right;
		}

		// 리턴하는 포인터의 up Point(부모 노드 위치)에 먼저, pParentNode(이전 노드 포인터 - 부모 위치)를 넣어줍니다.
		pRetNode->up = pParentNode;
#pragma endregion

#pragma region [ 삽입하는 노드의 위치 정하기 ]

		// 현재 부모 노드(RetNode->up == oldBuffer)가 pNullNode일 경우, 삽입한 노드는 트리의 pRoot노드가 됩니다.
		if (pParentNode == pNullNode)
		{
			pRoot = pRetNode;
		}
		else
		{
			// 트리의 최초 노드가 아닐 경우, 부모 노드( pParentNode == pRetNode->up)의 키 값과 비교해, 부모 노드에서의 위치 (Left Or Right)를 결정 후 연결해줍니다.
			if (pRetNode->GetKey() < pParentNode->GetKey() /* ==  pRetNode->up->GetKey()	*/)
				pParentNode->left = pRetNode;
			else
				pParentNode->right = pRetNode;

			// 위에 사용했던 포인트 재사용. 원본을 잘못쓰는 경우 방지.
			pTraversalNode = pRetNode;

			// 삽입 시, 균형 여부를 검사하고 관련 필요 처리를 하는 함수입니다. ( [Insert Case 1 ] 첫 삽입일때는 필요없습니다.(위의 if문, -- if (pParentNode == pNullNode) )
			_ChangeForInsert(pTraversalNode);
		}

#pragma endregion

		// 레드 - 블랙트리 법칙에 의해, 어떤 변환이 일어나든, 최종적으로 적색나무에서 루트 노드는 항상 검정색을 유지해야합니다.
		pRoot->color = BLACK;

		////(디버그)삽입 할때마다, 트리를 출력합니다.
		//this->PrintTree();

		// 삽입한 노드를 리턴해줍니다.
		//return pRetNode->GetData();
	};


	/*
	_ChangeForInsert(rbTreeNode<KEY, VALUE>* RetNode);
	- Insert 함수 내부에서 사용되며, 노드를 Insert 한 후에도, Red-Black Tree의 특징을 유지하기 위해 검사 및 처리를 해주는 함수입니다.

	#1. 관련 이론은 위키 백과, 레드-블랙 트리를 확인해 주세요! https://ko.wikipedia.org/wiki/%EB%A0%88%EB%93%9C-%EB%B8%94%EB%9E%99_%ED%8A%B8%EB%A6%AC

	인자 : Insert된 노드의 포인터

	출력 : void
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

		if (pUncleNode->color == RED /*&& pUncleNode != pNullNode // 이 조건은 빨간색에 포함. */)
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
	- 인자로 제공되는 노드의 포인터를 활용해, 해당 노드를 삭제합니다.

	#1. 기존 레드 - 블랙트리 방식과 조금 다른 점은 Copy Value가 아니라, Node 자체를 변경하는 점입니다.
	( 내부에서 Node에 대한 ptr를 활용할 때, 이에 대한 참조를 보장하기 위함.)

	!1. 내부에서 Node에 대한 메모리 회수(delete) 가 일어납니다.

	?1. deleted된 노드의 포인터가 pNullNode를 가르키면, nullptr 관련 에러를 방지하지 않을까?


	인자 : 제거하려는 노드의 포인터

	출력 : void
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
	- Delete 함수 내부에서 사용되며, 노드를 Delete 한 후에도, Red-Black Tree의 특징을 유지하기 위해 검사 및 처리를 해주는 함수입니다.

	#1. 관련 이론은 위키 백과, 레드-블랙 트리를 확인해 주세요! https://ko.wikipedia.org/wiki/%EB%A0%88%EB%93%9C-%EB%B8%94%EB%9E%99_%ED%8A%B8%EB%A6%AC

	인자 : Delete할 노드의 NextNode( Successor )의 ChildNode (dir - Right)

	출력 : void
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
			- pNullNode를 제외하고, 트리에 할당된 모든 노드들을 반납합니다.
		
		인자 : void
		출력 : void
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

	Predecessor를 구하는 함수, 사용되지 않습니다.


	인자 : Delete되어, PrevNode를 구해야하는 노드의 포인터

	출력 : Predecessor Node's Pointer
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
	참고(영어입니다.) : https://www.geeksforgeeks.org/inorder-successor-in-binary-search-tree/

	- 조금 말씀드리면, 제가 영어가 부족해 GetNextNode라는 함수명을 썻으나
	관련 명칭은 Predecessor, Successor이며 각각, 해당 노드의 좌측 서브트리의 최대값 (Predecessor), 해당 노드의 우측 서브트리의 최소값 (Successor) 입니다.

	이를 구해야할 경우는,
	삭제되는 노드가 두 개의 pNullNode가 아닌 자식을 가질 경우, 해당 노드의 자리에 어떤 노드가 와야하는 지를 구할 때,
	그 자리에 올 수 있는 노드는 Predecessor 또는 Successor가 있습니다.

	//
	이와 별개로 삭제하는 노드가 자식이 없을 경우, 고냥 삭제하면 되고,
	삭제하는 노드가 1개의 자식을 갖고 있을 경우, 해당 자식을 그 자리에 위치시키면 됩니다(like list)


	인자 : Delete되어, NextNode를 구해야하는 노드의 포인터

	출력 : Successor Node's Pointer
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

	- pInNode의 형제 노드의 포인터를 구하는 함수입니다.


	인자 : 노드의 포인터

	출력 : 형제 노드의 포인터
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
		//	std::cout << "Error - 형제 노드를 반환하는 함수에서 예외가 발생했습니다. \n";
		//}
	}

	/*
	_GetUncleNode(rbTreeNode<KEY, VALUE>* pInNode);

	- pInNode의 Uncle 노드의 포인터를 구하는 함수입니다.


	인자 : 노드의 포인터

	출력 : 삼촌 노드의 포인터
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
		//		std::cout << "Error - Uncle 노드를 반환하는 함수에서 예외가 발생했습니다. \n";
		//}
	}



	// ================================== Debug Function


	/*
	!1. 내부에는 재귀함수를 사용하고 있습니다. 트리 내부 노드의 개수가 높을 경우, 스택 오버플로우가 발생할 수 있습니다.
	(디버그 용도로만 사용하는 것을 추천드립니다.)
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
