#include"May_14.h"

BSTNode* BuyNode(DataType data)
{
	BSTNode* NewNode = (BSTNode*)malloc(sizeof(BSTNode));
	if (NULL == NewNode)
	{
		assert(0);
		return NULL;
	}
	NewNode->_data = data;
	NewNode->_pLeft = NULL;
	NewNode->_pRight = NULL;
	return NewNode;
}

void Swap(BSTNode** parent, BSTNode** child)
{
	DataType tmp = (*parent)->_data;
	(*parent)->_data = (*child)->_data;
	(*child)->_data = tmp;
}

// 初始化二叉搜索树
void InitBSTree(BSTNode** pRoot)
{
	assert(pRoot);
	*pRoot = NULL;
}

// 插入值为data的元素
void InsertBSTree(BSTNode** pRoot, DataType data)
{
	assert(pRoot);
	if (NULL == *pRoot)
	{
		*pRoot = BuyNode(data);
	}
	else
	{
		if(data > (*pRoot)->_data)
		{
			InsertBSTree(&(*pRoot)->_pRight, data);
		}
		else if (data < (*pRoot)->_data)
		{
			InsertBSTree(&(*pRoot)->_pLeft, data);
		}
		else
			return;
	}
}

// 删除值为data的结点
void DeleteBSTree(BSTNode** pRoot, DataType data)
{
	BSTNode* Del = FindBSTree(*pRoot, data);
	BSTNode* pPre = NULL;

	assert(pRoot);
	if (NULL == *pRoot)
		return;

	if (NULL == Del)
	{
		printf("该值不存在\n");
		return;
	}
	else
	{
		if (NULL == Del->_pLeft && NULL == Del->_pRight)
			free(Del);
		else if (NULL == Del->_pLeft)
		{
			Swap(&Del, &Del->_pRight);
			Del->_pLeft = Del->_pRight->_pLeft;
			Del->_pRight = Del->_pRight->_pRight;
			free(Del->_pRight->_pRight);
			Del->_pRight->_pRight = NULL;
		}
		else if (NULL == Del->_pRight)
		{
			Swap(&Del, &Del->_pLeft);
			Del->_pLeft = Del->_pLeft->_pLeft;
			Del->_pRight = Del->_pLeft->_pRight;
			free(Del->_pLeft->_pLeft);
			Del->_pLeft->_pLeft = NULL;
		}
		else
		{
			
			BSTNode* pCur = Del->_pRight;
			int flag = 0;
			while (pCur->_pLeft)
			{
				pPre = pCur;
				pCur = pCur->_pLeft;
				flag = 1;
			}
			if (flag)
			{
				Swap(&Del, &pCur);
				free(pCur);
				pPre->_pLeft = NULL;
			}
			else
			{
				Swap(&Del, &pCur);
				free(pCur);
				Del->_pRight = NULL;
			}
		}
	}
}

// 在二叉搜索树中查找值为data的结点
BSTNode* FindBSTree(BSTNode* pRoot, DataType data)
{
	if (NULL == pRoot)
		return NULL;

	if (pRoot->_data == data)
		return pRoot;

	if (data < pRoot->_data)
	{
		return FindBSTree(pRoot->_pLeft, data);
	}
	else if (data > pRoot->_data)
	{
		return FindBSTree(pRoot->_pRight, data);
	}
	return NULL;
}

// 中序遍历二叉搜索树
void PreOrder(BSTNode* pRoot)
{
	if (NULL == pRoot)
		return;

	if (pRoot)
	{
		PreOrder(pRoot->_pLeft);
		printf("%d ", pRoot->_data);
		PreOrder(pRoot->_pRight);
	}	
}

// 销毁二叉搜索树
void DestroyBSTree(BSTNode** pRoot)
{
	assert(pRoot);
	
	if ((*pRoot)->_pLeft)
	{
		DestroyBSTree(&((*pRoot)->_pLeft));
		if ((*pRoot)->_pRight)
			DestroyBSTree(&(*pRoot)->_pRight);
		else
			free(*pRoot);
	}
}

void Test()
{
	BSTNode* bst;
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	int i = 0;
	InitBSTree(&bst);
	for (; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		InsertBSTree(&bst, a[i]);
	}
	PreOrder(bst);
	printf("\n");
	DeleteBSTree(&bst, 5);
	DeleteBSTree(&bst, 7);
	DeleteBSTree(&bst, 3);
	PreOrder(bst);
	DestroyBSTree(&bst);
}