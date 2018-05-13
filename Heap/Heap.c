#include"May_12.h"
#include"PriorityQueue.h"


void _Adjustdown(Heap* hp, int parent);

int Less(int x,int y)
{
	return  x < y;
}

int Greater(int x, int y)
{
	return  x > y;
}

void HeapInit(Heap* hp, Compare cmp)
{
	assert(hp);
	hp->_size = 0;
	hp->_capacity = hp->_size;
	hp->comp = cmp;
}

void Swap(int* parent, int* child)
{
	int tmp = *parent;
	*parent = *child;
	*child = tmp;
}

void _Adjustup(Heap* hp, int child)
{
	int parent = (child - 1) >> 1;
	while (child)
	{
		if ((child + 1) < hp->_size && hp->comp(hp->_data[child + 1] , hp->_data[child]))
			++child;
		if (hp->comp(hp->_data[child], hp->_data[parent]))
		{
			Swap(&hp->_data[parent], &hp->_data[child]);
			child = parent;
			parent = (child - 1) >> 1;
		}
		else
			break;
	}
}

void _Adjustdown(Heap* hp, int parent)
{
	assert(hp);
	int child = (parent << 1) + 1;
	while (child < hp->_size)
	{
		if (child + 1 < hp->_size && hp->comp(hp->_data[child + 1], hp->_data[child]))
		{
			++child;
		}
		if (hp->comp(hp->_data[child], hp->_data[parent]))
		{
			Swap(&hp->_data[parent], &hp->_data[child]);
			parent = child;
			child = (parent << 1) + 1;
		}
		else
		{
			break;
		}
	}
}

//创建堆
void CreteHeap(Heap* hp, int arr[], int size)
{
	assert(hp);
	int i = 0;
	int child = 0;
	int parent = 0;
	hp->_data = (DataType*)malloc(sizeof(DataType)* size);
	if (NULL == hp->_data)
	{
		assert(0);
		return;
	}
	hp->_capacity = size;
	hp->_size = size;
	for (; i < size; ++i)
	{	
		hp->_data[i] = arr[i];
	}
	parent = size - 1;
	for (; parent >= 0; --parent)
	{
		_Adjustdown(hp, parent);
	}	
}

void InsertHeap(Heap* hp, DataType data)
{
	int parent = 0;
	assert(hp);
	if (hp->_size == hp->_capacity)
	{
		DataType* NewBase = (DataType*)realloc(hp->_data, hp->_capacity * 2 * sizeof(DataType));
		if (NULL == NewBase)
		{
			assert(0);
			return;
		}
		hp->_data = NewBase;
		hp->_capacity = hp->_capacity * 2;
	}
	hp->_data[hp->_size] = data;
	++hp->_size;
	for (; parent < hp->_size; ++parent)
	{
		_Adjustup(hp, parent);
	}
}

//返回堆顶元素
DataType HeapTop(Heap* hp)
{
	assert(hp);
	return hp->_data[0];
}

// 检测一个堆是否为空堆
int EmptyHeap(Heap* hp)
{
	assert(hp);
	if (0 == hp->_size)
		return 1;
	return 0;
}

// 获取堆中元素的个数
int SizeHeap(Heap* hp)
{
	assert(hp);
	return hp->_size;
}

// 删除堆顶元素
void DeleteHeap(Heap* hp)
{
	assert(hp);
	int parent;
	Swap(&hp->_data[0], &hp->_data[hp->_size - 1]);
	hp->_size;
	parent = hp->_size - 1;
	for (; parent >= 0; --parent)
	{
		_Adjustdown(hp, parent);
	}
}

// 销毁堆
void DestroyHeap(Heap* hp)
{
	assert(hp);
	free(hp->_data);
	hp->_data = NULL;
	hp->_size = 0;
	hp->_capacity = 0;
}

void Test()
{
	int arr[] = { 53, 17, 78, 9, 45, 65, 87, 23, 31 };
	Heap hp;
	HeapInit(&hp, Greater);
	CreteHeap(&hp, arr, sizeof(arr) / sizeof(arr[0]));
	InsertHeap(&hp, 99);
	InsertHeap(&hp, 101);
	InsertHeap(&hp, 3);
	InsertHeap(&hp, 0);
	InsertHeap(&hp, 119);
	InsertHeap(&hp, 167);
	printf("%d\n",HeapTop(&hp));
	PriorityQueue q;
	PriorityQueueInit(&q, Greater);
	PriorityQueuePush(&q, 7);
	PriorityQueuePop(&q);
	PriorityQueueDestroy(&q);
}
