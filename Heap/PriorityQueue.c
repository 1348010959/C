#include"PriorityQueue.h"

// 优先级队列初始化
void PriorityQueueInit(PriorityQueue* q, Compare com)
{
	HeapInit(&q->_hp, com);
}

// 向队列中插入元素
void PriorityQueuePush(PriorityQueue* q, DataType data)
{
	int arr[] = { data };
	if (PriorityQueueEmpty)
		CreteHeap(&q->_hp, arr, 1);
	else
	{
		InsertHeap(&q->_hp, data);
	}
}

// 删除优先级最高的元素
void PriorityQueuePop(PriorityQueue* q)
{
	DeleteHeap(&q->_hp);
}

// 获取队列中优先级最高的元素
int PriorityQueueSize(PriorityQueue* q)
{
	return HeapTop(&q->_hp);
}

// 检测优先级队列是否为空
int PriorityQueueEmpty(PriorityQueue* q)
{
	return EmptyHeap(&q->_hp);
}

// 获取堆顶的元素
DataType PriorityQueueTop(PriorityQueue* q)
{
	return HeapTop(&q->_hp);
}

// 销毁优先级队列
void PriorityQueueDestroy(PriorityQueue* q)
{
	DestroyHeap(&q->_hp);
}