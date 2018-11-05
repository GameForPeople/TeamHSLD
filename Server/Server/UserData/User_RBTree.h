#pragma once

#include "../PCH/stdafx.h"
#include "../UserData/UserData.h"

/*
Custom Red Black node Tree

	1. 적흑나무 템플릿 들어낼건지 어떨건지 고민. ( string 만큼 손해일 가능성 큼 )
	2. .... 디버깅 정확하지 않음.

*/

//declare
template <typename KEY, typename VALUE>
class rbTree;

template <typename KEY, typename VALUE>
class rbTreeNode {
	friend class rbTree<KEY, VALUE>;

	enum
	{
		RED = true,	//TRUE
		BLACK = false		//FALSE
	};

private:
	KEY							key;
	VALUE						value;

	bool						color; // false ( 0 ) == black , true (1) == red;

	rbTreeNode<KEY, VALUE>*		up;
	rbTreeNode<KEY, VALUE>*		left;
	rbTreeNode<KEY, VALUE>*		right;

public:
	// for NullNode, buffer
	rbTreeNode<KEY, VALUE>() : key(), value(), up(nullptr), left(nullptr), right(nullptr), color(false)
	{}

	// for Insert
	rbTreeNode<KEY, VALUE>(const KEY& InKEY, const VALUE& InValue)
		: key(InKEY), value(InValue), up(nullptr), left(nullptr), right(nullptr), color(false)
	{}

	~rbTreeNode<KEY, VALUE>() = default;

public:
	KEY GetKey() const
	{
		return key;
	}

	VALUE GetValue() const
	{
		return value;
	}

	// like pair (same Function --> GetKey() )
	KEY First() const
	{
		return key;
	}

	// like pair (same Function --> GetValue() )
	VALUE Second() const
	{
		return value;
	}
};

template <typename KEY, typename VALUE>
class rbTree
{
	enum
	{
		RED = true,	//TRUE
		BLACK = false		//FALSE
	};

	rbTreeNode<KEY, VALUE>* root;
	rbTreeNode<KEY, VALUE>* nullNode;

public:
	rbTree<KEY, VALUE>() : nullNode(new rbTreeNode<KEY, VALUE>()) //, root(nullNode) // <-- Error
	{
		root = nullNode;
		/* In Node Construct rbTreeNode::rbTreeNode()
		nullNode->up = 0;
		nullNode->left = 0;
		nullNode->right = 0;
		nullNode->color = BLACK;
		*/
	};

	~rbTree<KEY, VALUE>()
	{
		// 다른 Node들에 대한 처리가 필요할 수 있음. 고냥 딤질 수 있음. 마 알아서 delete 해라 마.

		if (root != nullptr)
			delete root;

		if (nullNode != nullptr)
			delete nullNode;
	};

public:
	rbTreeNode<KEY, VALUE>*	Search(const KEY& InKey, bool& RetResult) const;			// 해당 Key값으로 검색하여, True시 노드 포인터 리턴, False시 없음(nullNode Return). 

	rbTreeNode<KEY, VALUE>* Insert(const KEY& InKey, const VALUE& InValue);		// 해당 key값과, Value 값을 가지고, 내부에서 할당하여 트리에 삽입 후, 해당 노드에 대한 포인터 리턴.
	void					Delete(rbTreeNode<KEY, VALUE>* DeletedNode);		// 해당 노드를 내부에서 delete 해줌. 

private:
	void					ChangeForInsert(rbTreeNode<KEY, VALUE>* RetNode);
	void					ChangeForDelete(rbTreeNode<KEY, VALUE>* RetNode);

	void					LeftRotate(rbTreeNode<KEY, VALUE>* RetNode);
	void					RightRotate(rbTreeNode<KEY, VALUE>* RetNode);

	rbTreeNode<KEY, VALUE>*	GetNextNode(rbTreeNode<KEY, VALUE>* InNode);

public:
	void					PrintTree();

private:
	void					PrintNodes(rbTreeNode<KEY, VALUE>* x);
};


//////////////////////////////////////////////////////////////////////////////
// Public Function
//////////////////////////////////////////////////////////////////////////////

/*
Search(const KEY& InKey, bool& RetResult);
- 주어진 키 값을 가지고, 트리에서 노드를 찾는 함수.

인자 : 탐색하는 노드의 키 값, 탐색 성공 여부

출력 : 노드 포인터(리턴값), 탐색 성공 여부(인자)
*/

template <typename KEY, typename VALUE>
rbTreeNode<KEY, VALUE>* rbTree<KEY, VALUE>::Search(const KEY& InKey, bool& RetResult) const
{
	RetResult = false;

	rbTreeNode<KEY, VALUE>*  RetNodeBuffer = root;

	while (true)
	{
		if (RetNodeBuffer == nullNode)
		{
			RetResult = false;
			break;
		}

		if (InKey == RetNodeBuffer->GetKey())
		{
			RetResult = true;
			break;
		}

		if (InKey < RetNodeBuffer->GetKey())
			RetNodeBuffer = RetNodeBuffer->left;
		else
			RetNodeBuffer = RetNodeBuffer->right;
	}
	return RetNodeBuffer;
};


/*
Insert(const KEY& InKey, const VALUE& InValue);
- 인자로 제공되는 해당 키, 결과 값을 가지는 노드를 삽입하는 함수.
#1. 동일한 키값에 대하여 Insert를 요청할 경우, 오류의 원인이 될 수 있습니다.
#2. 내부에서 Node에 대한 할당(new) 가 일어납니다.

인자 : 노드의 키, 데이터

출력 : 해당 노드에 대한 포인터 제공.
*/

template <typename KEY, typename VALUE>
rbTreeNode<KEY, VALUE>* rbTree<KEY, VALUE>::Insert(const KEY& InKey, const VALUE& InValue)
{
	rbTreeNode<KEY, VALUE>* pRetNode = new rbTreeNode<KEY, VALUE>(InKey, InValue);

	rbTreeNode<KEY, VALUE>* pNodeA = root;
	rbTreeNode<KEY, VALUE>* pNodeB = nullNode;

	while (pNodeA != nullNode)
	{
		pNodeB = pNodeA;

		if (pRetNode->GetKey() /* == InKey */ < pNodeA->GetKey())
			pNodeA = pNodeA->left;
		else
			pNodeA = pNodeA->right;
	}

	pRetNode->up = pNodeB;

	if (pNodeB == nullNode)
		root = pRetNode;
	else
	{
		if (pRetNode->GetKey() < pNodeB->GetKey())
			pNodeB->left = pRetNode;
		else
			pNodeB->right = pRetNode;
	}

	pRetNode->left = nullNode;
	pRetNode->right = nullNode;
	pRetNode->color = RED;

	ChangeForInsert(pRetNode);

	root->color = BLACK;

	return pRetNode;
};


/*
Delete(rbTreeNode<KEY, VALUE>* DeletedNode);
- 인자로 제공되는 노드의 포인터를 활용해, 해당 노드를 삭제합니다.
#1. 내부에서 Node에 대한 메모리 회수(delete) 가 일어납니다.
?1. deleted된 노드의 포인터가 NullNode를 가르키면, nullptr 관련 에러를 방지하지 않을까?

인자 : 노드의 포인터

출력 : void

*/

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::Delete(rbTreeNode<KEY, VALUE>* DeletedNode)
{
	// 제거하려는 노드가 nullNode가 아닌지 확인합니다. (nullNode 제거하면 클납니다.)
	if (DeletedNode != nullNode)
	{
		rbTreeNode<KEY, VALUE>* nodeA = nullNode;
		rbTreeNode<KEY, VALUE>* nodeB = nullNode;

		if (DeletedNode->left == nullNode || DeletedNode->right == nullNode)
			nodeB = DeletedNode;
		else
			nodeB = GetNextNode(DeletedNode);

		if (nodeB->left != nullNode)
			nodeA = nodeB->left;
		else
			nodeA = nodeB->right;

		nodeA->up = nodeB->up;

		if (nodeB->up == nullNode)
			root = nodeA;
		else
		{
			if (nodeB == nodeB->up->left)
				nodeB->up->left = nodeA;
			else
				nodeB->up->right = nodeA;
		}

		if (nodeB != DeletedNode)
			DeletedNode->key = nodeB->key;

		if (nodeB->color == BLACK)
			ChangeForDelete(nodeA);

		nodeA->color = BLACK;
		delete nodeB;
	}
};


// Private Function

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::ChangeForInsert(rbTreeNode<KEY, VALUE>* RetNode)
{
	while (RetNode->up->color == RED)
	{
		if (RetNode->up == RetNode->up->up->left)
			// Uncle Node 의 위치 찾기. --> Uncle이 Right!
		{
			rbTreeNode<KEY, VALUE>* uncleNode = RetNode->up->up->right;

			if (uncleNode->color == RED)
			{
				// 색 변경. (Recoloring)
				RetNode->up->color = BLACK;
				uncleNode->color = BLACK;

				RetNode->up->up->color = RED;

				// Red 색으로, 부모의 부모노드 변환 시, 해당 노드에 대한 Double Red Test 필요.
				RetNode = RetNode->up->up;
			}
			else {
				// 모습 변경 (Restructuring)
				if (RetNode == RetNode->up->right)
				{
					RetNode = RetNode->up;
					LeftRotate(RetNode);
				}

				RetNode->up->color = BLACK;
				RetNode->up->up->color = RED;
				RetNode->up->right->color = BLACK;
				RightRotate(RetNode->up->up);
			}
		}
		else
			// Uncle Node 의 위치 찾기. --> Uncle이 Left!
		{
			rbTreeNode<KEY, VALUE>* uncleNode = RetNode->up->up->left;

			if (uncleNode->color == RED)
			{
				// 색 변경. (Recoloring)
				RetNode->up->color = BLACK;
				uncleNode->color = BLACK;

				RetNode->up->up->color = RED;

				// Red 색으로, 부모의 부모노드 변환 시, 해당 노드에 대한 Double Red Test 필요.
				RetNode = RetNode->up->up;
			}
			else {
				// 모습 변경 (Restructuring)
				if (RetNode == RetNode->up->left)
				{
					RetNode = RetNode->up;
					RightRotate(RetNode);
				}

				RetNode->up->color = BLACK;
				RetNode->up->up->color = RED;
				RetNode->up->right->color = BLACK;
				LeftRotate(RetNode->up->up);
			}
		}
	}
};

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::ChangeForDelete(rbTreeNode<KEY, VALUE>* RetNode)
{
	while (RetNode != root && RetNode->color == BLACK)
	{
		rbTreeNode<KEY, VALUE>* buffer = 0;

		if (RetNode->up->left == RetNode)
		{
			buffer = RetNode->up->right;

			if (buffer->color == RED)
			{
				buffer->color = BLACK;
				RetNode->up->color = RED;
				LeftRotate(RetNode->up);
				buffer = RetNode->up->right;
			}

			if (buffer->left->color == BLACK && buffer->right->color == BLACK)
			{
				buffer->color = RED;
				RetNode = RetNode->up;
			}
			else
			{
				if (buffer->right->color == BLACK)
				{
					buffer->left->color = BLACK;
					buffer->color = RED;
					RightRotate(buffer);
					buffer = RetNode->up->right;
				}
				buffer->color = RetNode->up->color;
				RetNode->up->color = BLACK;
				buffer->right->color = BLACK;
				LeftRotate(RetNode->up);
				RetNode = root;
			}
		}
		else
		{
			buffer = RetNode->up->left;
			if (buffer->color == RED)
			{
				buffer->color = BLACK;
				RetNode->up->color = RED;
				RightRotate(RetNode->up);
				buffer = RetNode->up->left;
			}
			if (buffer->right->color == BLACK && buffer->left->color == BLACK)
			{
				buffer->color = RED;
				RetNode = RetNode->up;
			}
			else
			{
				if (buffer->left->color == BLACK)
				{
					buffer->right->color = BLACK;
					buffer->color = RED;
					LeftRotate(buffer);
					buffer = RetNode->up->left;
				}

				buffer->color = RetNode->up->color;
				RetNode->up->color = BLACK;
				buffer->left->color = BLACK;
				RightRotate(RetNode->up);
				RetNode = root;
			}
		}
	}
};

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::LeftRotate(rbTreeNode<KEY, VALUE>* RetNode)
{
	rbTreeNode<KEY, VALUE>* rightChildNode = RetNode->right;

	if (RetNode->up == nullNode) 
		root = rightChildNode;
	else
	{
		if (RetNode == RetNode->up->left)
			RetNode->up->left = rightChildNode;
		else
			RetNode->up->right = rightChildNode;
	}

	rightChildNode->up = RetNode->up;
	RetNode->right = rightChildNode->left;
	rightChildNode->left->up = RetNode;
	rightChildNode->left = RetNode;
	RetNode->up = rightChildNode;
};

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::RightRotate(rbTreeNode<KEY, VALUE>* RetNode)
{
	rbTreeNode<KEY, VALUE>* leftChildNode = RetNode->left;
	if (RetNode->up == nullNode) 
		root = leftChildNode;
	else
	{
		if (RetNode == RetNode->up->left)
			RetNode->up->left = leftChildNode;
		else
			RetNode->up->right = leftChildNode;
	}

	leftChildNode->up = RetNode->up;
	RetNode->left = leftChildNode->right;
	leftChildNode->right->up = RetNode;
	leftChildNode->right = RetNode;
	RetNode->up = leftChildNode;
};

template <typename KEY, typename VALUE>
rbTreeNode<KEY, VALUE>*	rbTree<KEY, VALUE>::GetNextNode(rbTreeNode<KEY, VALUE>* InNode)
{
	if (InNode->right != nullNode) {
		while (InNode->left != nullNode)
			InNode = InNode->left;
		return InNode;
	}

	rbTreeNode<KEY, VALUE>* bufferNode = InNode->up;
	while (bufferNode != nullNode && InNode == bufferNode->right) {
		InNode = bufferNode;
		bufferNode = bufferNode->up;
	}
	return bufferNode;
};


//Debug Function

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::PrintTree()
{
	if (root != nullNode)
	{
		std::cout << "rootNode's Key : " << root->GetKey() << "\n";
		std::cout << "Node - Key   Color     Up     Left     Right \n";
		PrintNodes(root);
	}
	else
		cout << "Tree Root is NullNode (VOID TREE) \n";
}

template <typename KEY, typename VALUE>
void rbTree<KEY, VALUE>::PrintNodes(rbTreeNode<KEY, VALUE>* x)
{
	if (x->left != nullNode)
		PrintNodes(x->left);
	if (x != nullNode) {
		std::cout << "       ";

		cout << x->key << ' ';

		std::cout << "    ";

		if (x->color == RED)
			cout << "RED   ";
		else
			cout << "BLACK ";

		std::cout << "     ";

		if (x->up != nullNode)
			cout << x->up->key << ' ';
		else
			cout << "NULL ";

		std::cout << "     ";

		if (x->left != nullNode)
			cout << x->left->key << ' ';
		else
			cout << "NULL ";

		std::cout << "     ";

		if (x->right != nullNode)
			cout << x->right->key << ' ';
		else
			cout << "NULL ";
	}
	cout << endl;
	if (x->right != nullNode)
		PrintNodes(x->right);
}