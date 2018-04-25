//头文件
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef int DataType;

typedef struct ListNode{
	struct ListNode *_pNext;
	DataType _data;
}Node, *PNode;


void ListNodeInit(PNode* PHead);	//初始化单链表

PNode BuyNode(DataType data);	//创建新节点

void ListPushBack(PNode* PHead, DataType data);	//尾插

void ListPopBack(PNode* PHead);	//尾删

void ListPushFront(PNode* PHead, DataType data);	//头插

void ListPopFront(PNode* PHead);//头删

PNode ListFind(PNode PHead, DataType data);	// 在链表中查找值为data的元素，找到后返回值为data的结点 

void ListInsert(PNode* pHead, PNode pos, DataType data);	// 在pos位置插入值为data的结点

void ListErase(PNode* pHead, PNode pos);	// 删除pos位置的结点

int ListEmpty(PNode pHead);	//判断链表是否为空

int SListSize(PNode pHead);	// 获取链表中值data的结点 

void SListDestroy(PNode* pHead);	// 销毁聊表


PNode FindNode(PNode PHead, int pos);	//查找已经存在的链表内是否存在第pos位结点
void ListPrint(PNode PHead);	//打印结点内容



/////////////////////////////////////////////////////////////////////////////////
void ReverseListPrint(PNode PHead);	//逆序打印顺序表

PNode FindMiddleNode(PNode PHead);	// 查找链表的中间结点，要求只能遍历一次链表

PNode FindLastKNode(PNode pHead, int K);	// 查找链表的倒数第K个结点，要求只能遍历一次链表

void JosephCircle(PNode* pHead, const int M);	// 用单链表实现约瑟夫环

void ReverseList(PNode* PHead);	// 链表的逆置--三个指针

PNode MergeSList(PNode pHead1, PNode pHead2);	// 合并两个有序单链表，合并后依然有序

int IsSListCross(PNode PHead1, PNode PHead2);	// 判断两个单链表是否相交---链表不带环

PNode GetCorssNode(PNode PHead1, PNode PHead2);	// 求两个单链表相交的交点---链表不带环

void BubbleSortList(PNode PHead);	//用冒泡的思想对链表进行排序


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//测试文件
#define _CRT_SECURE_NO_WARNINGS 1
#include"List.h"

void Test()
{
	PNode PHead;
	PNode tmp;
	ListNodeInit(&PHead);
	ListPushBack(&PHead, 1);
	ListPushBack(&PHead, 2);
	ListPushBack(&PHead, 3);
	ListPushBack(&PHead, 4);
	ListPushBack(&PHead, 5);
//	ReverseListPrint(PHead);
	tmp = FindNode(PHead, 4);
//	ListErase(&PHead, tmp);
	ListPrint(PHead);
	tmp = FindMiddleNode(PHead);
	ListPrint(tmp);
	tmp = FindLastKNode(PHead, 5);
	if (!tmp)
		ListPrint(tmp);
}

void TestJosephCircle()
{
	PNode PHead;
	ListNodeInit(&PHead);
	ListPushBack(&PHead, 1);
	ListPushBack(&PHead, 2);
	ListPushBack(&PHead, 3);
	ListPushBack(&PHead, 4);
	ListPushBack(&PHead, 5);
//	JosephCircle(&PHead, 3);
	ListPrint(PHead);
	ReverseList(&PHead);
	ListPrint(PHead);
}

void TestMerge()
{
	PNode PHead1, PHead2;
	PNode tmp;
	ListNodeInit(&PHead1);
	ListPushBack(&PHead1, 1);
	ListPushBack(&PHead1, 1);
	ListPushBack(&PHead1, 4);
	ListPushBack(&PHead1, 7);
	ListPushBack(&PHead1, 9);
	ListPrint(PHead1);

	ListNodeInit(&PHead2);
	ListPushBack(&PHead2, 2);
	ListPushBack(&PHead2, 4);
	ListPushBack(&PHead2, 6);
	ListPushBack(&PHead2, 8);
	ListPushBack(&PHead2, 10);
	ListPrint(PHead2);
	tmp = MergeSList(PHead1, PHead2);
	ListPrint(tmp);
}

void TestIsSListCross()
{
	PNode PHead1, PHead2;
	PNode tmp,tmp2;
	int ret = 0;
	ListNodeInit(&PHead1);
	ListPushBack(&PHead1, 1);
	ListPushBack(&PHead1, 1);
	ListPushBack(&PHead1, 4);
	ListPushBack(&PHead1, 7);
	ListPushBack(&PHead1, 9);
	tmp = FindNode(PHead1, 5);

	ListNodeInit(&PHead2);
	ListPushBack(&PHead2, 2);
	ListPushBack(&PHead2, 4);
	ListPushBack(&PHead2, 6);
	tmp2 = FindNode(PHead2, 3);
	tmp->_pNext = tmp2;
	ret = IsSListCross(PHead1, PHead2);
	if (ret == 1)
	{
		printf("相交!!!\n");
	}
	else if (ret == 0)
	{
		printf("不相交!!!\n");
	}
	tmp = GetCorssNode(PHead1, PHead2);
	if (tmp)
	{
		printf("%d\n", tmp->_data);
	}
}

void TestBubblesort()
{
	PNode PHead;
	ListNodeInit(&PHead);
	ListPushBack(&PHead, 1);
	ListPushBack(&PHead, 3);
	ListPushBack(&PHead, 6);
	ListPushBack(&PHead, 2);
	ListPushBack(&PHead, 5);
	BubbleSortList(PHead);
	ListPrint(PHead);
}

int main()
{
	//Test();
	//TestJosephCircle();
	//TestMerge();
	//TestIsSListCross();
	TestBubblesort();
	system("pause");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//函数功能模块
#include"List.h"

//初始化链表
void ListNodeInit(PNode* pHead)
{
	assert(pHead);
	*pHead = NULL;
}

//申请节点
PNode BuyNode(DataType data)
{
	PNode NewNode = (PNode)malloc(sizeof(Node));	//sizeof(Node)  内部一定是结点的大小，而不是指针的大小

	if (NULL == NewNode)	//虽然几率很小，但是万一申请失败程序就会崩溃，所以记得加容错机制
	{
		assert(0);
		return NULL;
	}

	NewNode->_data = data;
	NewNode->_pNext = NULL;
	return NewNode;
}

//尾插
void ListPushBack(PNode* PHead, DataType data)
{
	assert(PHead);	//PHead内存的链表的地址，如果为空，则说明链表不存在

	if (NULL == *PHead)	//检查链表是否为空
		*PHead = BuyNode(data);
	else
	{
		PNode pCur = *PHead;

		//找出最后一个结点
		//while(pCur)也是寻找最后一个结点的一种方法，不过要保存它的前一个结点
		while (pCur->_pNext)
		{
			pCur = pCur->_pNext;
		}
		pCur->_pNext = BuyNode(data);
	}
}

//尾删
void ListPopBack(PNode* PHead)
{
	PNode Del = NULL;
	PNode prev = NULL;
	assert(PHead);

	if (NULL == *PHead)	//链表为空，没有结点需要删除
	{
		return;
	}

	else if ((*PHead)->_pNext)
	{
		Del = *PHead;
		while (Del->_pNext)
		{
			prev = Del;
			Del = Del->_pNext;
		}
		free(Del);
		prev->_pNext = NULL;
	}
	else if (*PHead)
	{
		free((*PHead));
		*PHead = NULL;
	}
}

//头插
void ListPushFront(PNode* PHead, DataType  data)
{
	assert(PHead);

	PNode NewNode;
	if (NULL == *PHead)
	{
		NewNode = BuyNode(data);
		NewNode->_pNext = NULL;
		*PHead = NewNode;
	}
	else
	{
		NewNode = BuyNode(data);
		NewNode->_pNext = *PHead;
		*PHead = NewNode;
	}
}

//头删
void ListPopFront(PNode* PHead)
{
	PNode Del = (*PHead);
	PNode Pre = NULL;

	assert(PHead);

	if (NULL == PHead)
	{
		return;
	}

	if ((*PHead)->_pNext)
	{
		*PHead = (*PHead)->_pNext;
		free(Del);
		Del = NULL;
	}
	else if (*PHead)
	{
		free(Del);
		*PHead = NULL;
	}
}

// 在链表中查找值为data的元素，找到后返回值为data的结点 
PNode ListFind(PNode PHead, DataType data)
{
	PNode PCur = NULL;
	assert(PHead);

	if (NULL == PHead)
		return NULL;

	PCur = PHead;

	while (PCur)
	{
		if (PCur->_data == data)
		{
			return PCur;
		}
		PCur = PCur->_pNext;
	}

	return NULL;
}

// 在pos位置插入值为data的结点
void ListInsert(PNode* PHead, PNode pos, DataType data)
{
	assert(PHead);

	if (NULL == (*PHead))	//	链表为空,自动头插
		ListPushFront(PHead, data);
	if (pos == NULL)	//插入位置为空，缺省为头插
		ListPushFront(PHead, data);
	else if (!pos->_pNext)	//插入位置为链表末尾
	{
		ListPushBack(PHead, data);
	}
	else
	{
		PNode PCur = NULL;
		PNode NewNode = NULL;
		DataType tmp;

		NewNode = BuyNode(data);
		NewNode->_pNext = pos->_pNext;
		pos->_pNext = NewNode;
		tmp = pos->_data;
		pos->_data = data;
		NewNode->_data = tmp;
	}

}

// 删除pos位置的结点
void ListErase(PNode* PHead, PNode pos)
{
	PNode Del = NULL;
	PNode PPre = NULL;
	assert(PHead);

	if (NULL == *PHead || NULL == pos)
		return;

	Del = *PHead;
	if (pos->_pNext)
	{
		Del = pos->_pNext;
		pos->_data = Del->_data;
		pos->_pNext = Del->_pNext;
		free(Del);
		return;
	}
	while (Del && Del != pos)
	{
		PPre = Del;
		Del = Del->_pNext;
	}
	if (Del->_pNext != NULL)
	{
		PPre->_pNext = Del->_pNext;
		free(Del);
		Del = NULL;
	}
	else
	{
		free(Del);
		PPre->_pNext = NULL;
	}
}

int ListEmpty(PNode pHead)
{
	if (NULL == pHead)
	{
		printf("链表为空\n");
		return 0;
	}
	else
	{
		return 1;
	}
}

// 销毁链表
void SListDestroy(PNode* PHead)
{
	PNode Des;
	assert(PHead);

	if (NULL == (PHead))
		return;

	Des = *PHead;
	while (Des)
	{
		Des = (*PHead)->_pNext;
		free(*PHead);
		if (Des)
			(*PHead) = Des;
	}
	*PHead = NULL;
}



//查找链表中是否存在第pos个结点
PNode FindNode(PNode PHead, int pos)
{
	PNode PCur = NULL;
	PNode PPre = NULL;
	if (NULL == PHead)
	{
		return NULL;
	}

	PCur = PHead;
	while (pos && PCur)
	{
		PPre = PCur;
		PCur = PCur->_pNext;
		pos--;
	}

	if (pos == 0)
		return PPre;
	else
		return NULL;
}

//打印
void ListPrint(PNode PHead)
{
	if (NULL == PHead)
		return;

	while (PHead)
	{
		printf("%d ", PHead->_data);
		PHead = PHead->_pNext;
	}
	printf("\n");
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 逆序打印单链表 
void ReverseListPrint(PNode PHead)
{
	if (NULL == PHead)
		return;

	if (PHead)
	{
		ReverseListPrint(PHead->_pNext);
		printf("%d ", PHead->_data);
	}
}

// 查找链表的中间结点，要求只能遍历一次链表
PNode FindMiddleNode(PNode PHead)
{
	PNode slow = NULL;
	if (NULL == PHead)
		return NULL;

	slow = PHead;
	while (PHead && PHead->_pNext)
	{
		PHead = PHead->_pNext;
		PHead = PHead->_pNext;
		if (!PHead)	//如果为偶数个结点，返回中间的前一个结点
		{
			return slow;
		}
		slow = slow->_pNext;
	}
	return slow;
}

// 查找链表的倒数第K个结点，要求只能遍历一次链表
PNode FindLastKNode(PNode PHead, int K)
{
	PNode slow = NULL;

	if (NULL == PHead)
		return NULL;

	slow = PHead;
	while (PHead && --K)
	{
		PHead = PHead->_pNext;
	}
	if (!PHead && K!=0)	//如果K！=0的时候PHead为空，则返回
	{
		return NULL;
	}

	while (PHead->_pNext)
	{
		slow = slow->_pNext;
		PHead = PHead->_pNext;
	}
	return slow;
}

// 用单链表实现约瑟夫环
void JosephCircle(PNode* PHead, const int M)
{
	PNode PTail = NULL;
	int tmp = 0;
	assert(PHead);

	if (NULL == *PHead)
	{
		return;
	}

	PTail = (*PHead);
	while (PTail->_pNext)
	{
		PTail = PTail->_pNext;
	}

	PTail->_pNext = *PHead;	//成环
	while ((PTail)->_pNext != PTail)
	{
		tmp = M;
		while (tmp)
		{
			PTail = PTail->_pNext;
			tmp--;
		}
		printf("%d ", PTail->_data);
		ListErase(&PTail, PTail);
	}
	(*PHead) =  PTail;
	(*PHead)->_pNext = NULL;
}

void ReverseList(PNode* PHead)	// 链表的逆置--三个指针
{
	PNode PPre = NULL;
	PNode PCur = NULL;
	PNode PNext = NULL;
	assert(PHead);

	if (NULL == *PHead || (*PHead)->_pNext == NULL)
		return;

	PCur = *PHead;
	while (PCur)
	{
		PNext = PCur->_pNext;
		PCur->_pNext = PPre;
		PPre = PCur;
		PCur = PNext;
	}
	*PHead = PPre;
}

// 合并两个有序单链表，合并后依然有序
PNode MergeSList(PNode PHead1, PNode PHead2)
{
	PNode NewHead;
	NewHead = NULL;
	if (NULL == PHead1 || NULL == PHead2)
		return NULL;

	while (PHead1 && PHead2)
	{
		if ((PHead1->_data) >= (PHead2->_data))
		{
			ListPushBack(&NewHead, PHead2->_data);
			PHead2 = PHead2->_pNext;
		}
		else if ((PHead1->_data) < (PHead2->_data))
		{
			ListPushBack(&NewHead, PHead1->_data);
			PHead1 = PHead1->_pNext;
		}
		
	}

	while (PHead1)
	{
		ListPushBack(&NewHead, PHead1->_data);
		PHead1 = PHead1->_pNext;
	}
	while (PHead2)
	{
		ListPushBack(&NewHead, PHead2->_data);
		PHead2 = PHead2->_pNext;
	}

	return NewHead;
}

PNode FindBack(PNode PHead)
{
	PNode PCur = NULL;
	PNode PPre = NULL;
	if (!PHead)
	{
		return NULL;
	}

	PCur = PHead;
	while (PCur->_pNext)
	{
		PPre = PCur;
		PCur = PCur->_pNext;
	}
	return PCur;
}

// 判断两个单链表是否相交---链表不带环
int IsSListCross(PNode PHead1, PNode PHead2)
{
	PNode PCur = NULL;
	PNode PCur2 = NULL;

	if (NULL == PHead1 || NULL == PHead2)
		assert(0);

	PCur = FindBack(PHead1);
	PCur2 = FindBack(PHead2);
	if (PCur->_pNext == PCur2->_pNext)
	{
		return 1;
	}
	else
		return 0;
}

// 求两个单链表相交的交点---链表不带环
PNode GetCorssNode(PNode PHead1, PNode PHead2)
{
	PNode PCur = NULL;
	PNode PCur2 = NULL;

	if (NULL == PHead1 || NULL == PHead2)
		assert(0);

	if (IsSListCross(PHead1, PHead1))
	{
		int count = 0;
		int count2 = 0;
		int tmp = 0;
		PCur = PHead1;
		while (PCur)
		{
			PCur = PCur->_pNext;
			++count;
		}
		PCur2 = PHead2;
		while (PCur2)
		{
			PCur2 = PCur2->_pNext;
			++count2;
		}
		tmp = count - count2;
		PCur = PHead1;
		PCur2 = PHead2;
		if (tmp>0)
		{
			while (tmp--)
			{
				PCur = PCur->_pNext;
			}
		}
		else if (tmp<0)
		{
			while (tmp++)
			{
				PCur2 = PCur2->_pNext;
			}
			PCur = PHead1;
		}
		while (PCur != PCur2)
		{
			PCur = PCur->_pNext;
			PCur2 = PCur2->_pNext;
		}
		return PCur;
	}

	return NULL;
}

//用冒泡的思想对链表进行排序
void BubbleSortList(PNode PHead)
{
	PNode PTail = NULL;
	PNode PCur;
	int flag = 0;

	if (!PHead || !(PHead->_pNext))
		return;

	while (PTail != PHead)
	{
		PCur = PHead;
		flag = 0;
		while (PCur->_pNext != PTail)
		{
			if (PCur->_data > PCur->_pNext->_data)
			{
				DataType tmp;
				tmp = PCur->_data;
				PCur->_data = PCur->_pNext->_data;
				PCur->_pNext->_data = tmp;
				flag = 1;
			}

			PCur = PCur->_pNext;
		}

		if (0 == flag)
			break;
	}
}
