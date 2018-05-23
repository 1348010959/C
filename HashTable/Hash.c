#define _CRT_SECURE_NO_WARNINGS 1
#include"Hash.h"

//哈希函数
DataType HashFunc(HashTable* ht, DataType data)
{
	assert(ht);
	return data % ht->_capacity;
}

//检测容量
int CheckCapacity(HashTable* ht)
{
	assert(ht);

	if ((ht->_total * 10) / (ht->_capacity) > 7)
		return 1;

	return 0;
}

//线性探测
int IsLineDete(HashTable* ht, DataType HashAddr)
{
	HashAddr = HashAddr + 1;
	if (HashAddr == ht->_capacity)
		HashAddr = 0;

	return HashAddr;
}

//二次探测
int DeteTwo(HashTable* ht, int HashAddr, int i)
{
	HashAddr = HashAddr + 2 * i + 1;

	//越界
	if (HashAddr >= ht->_capacity)
		HashAddr = HashAddr%ht->_capacity;

	return HashAddr;
}

void _Swap(int* newht, int* ht)
{
	int temp = *newht;
	*newht = *ht;
	*ht = temp;
}

//交换哈希表
void Swap(HashTable* newht, HashTable* ht)
{
	assert(newht);
	assert(ht);

	Elem* pTemp = newht->_table;
	newht->_table = ht->_table;
	ht->_table = pTemp;

	_Swap(&newht->_capacity, &ht->_capacity);
	_Swap(&newht->_size, &ht->_size);
	_Swap(&newht->_total, &ht->_total);
}

void BuyCapacity(HashTable* ht)
{
	assert(ht);
	
	HashTable newht;
	int i = 0;
	int NewCapacity = GetCapacity(ht->_capacity);

	//初始化新的哈希表
	HashTableInit(&newht, NewCapacity, ht->_Dete, ht->_setData);

	//拷贝元素
	for (; i < ht->_capacity; ++i)
	{
		if (ht->_table[i]._state = EXIST)
			HashTableInsert(&newht, ht->_table[i]._data);
	}

	//交换
	Swap(&newht, ht);
	HashTableDestroy(&newht);
}

//初始化
void HashTableInit(HashTable* ht,int capacity, IsLine _Dete, PDT setData)
{
	int i = 0;
	assert(ht);
	
	capacity = GetCapacity(capacity);
	ht->_table = (Elem*)malloc(sizeof(Elem)* capacity);
	if (NULL == ht->_table)
		return;

	for (; i < capacity; ++i)
	{
		ht->_table[i]._state = EMPTY;
	}

	ht->_capacity = capacity;
	ht->_size = 0;
	ht->_total = 0;
	ht->_setData = setData;
	ht->_Dete = _Dete;
}

//插入
void HashTableInsert(HashTable* ht, DataType data)
{
	assert(ht);
	DataType HashAddr = 0;
	int i = 0;
	int NewData = 0;

	//判断负载因子
	if (CheckCapacity(ht))
	{
		BuyCapacity(ht);
	}

	NewData = ht->_setData(data);
	HashAddr = HashFunc(ht, NewData);

	while (EMPTY != ht->_table[i]._state)
	{
		if (ht->_table[HashAddr]._state == EMPTY)
		{
			if (ht->_table[i]._data = data)
				return;
		}

		//线性探测
		if (ht->_Dete == _IsLine)
			HashAddr = IsLineDete(ht, HashAddr);
		else
			HashAddr = DeteTwo(ht, HashAddr, ++i);
	}

	//状态为空，直接插入
	ht->_table[HashAddr]._data = data;
	ht->_table[HashAddr]._state = EXIST;
	++ht->_size;
	++ht->_total;
}
void HashTableDelete(HashTable* ht, DataType data)
{
	assert(ht);
	int ret = HashFind(ht, data);
	if (ret != -1)
	{
		ht->_table[ret]._state = DELETE;
		--ht->_size;
	}
}

int HashTableSize(HashTable* ht)
{
	assert(ht);

	return ht->_size;
}

int HashTableEmpty(HashTable* ht)
{
	assert(ht);

	return 0 == ht->_size;
}

int HashFind(HashTable* ht, DataType data)
{
	assert(ht);
	int starAddr = 0;
	DataType HashAddr = 0;
	int i = 0;

	int NewData = ht->_setData(data);
	HashAddr = HashFunc(ht, NewData);

	starAddr = HashAddr;

	while (EMPTY != ht->_table[HashAddr]._state)
	{
		if (EXIST == ht->_table[HashAddr]._state)
		{
			if (data == ht->_table[HashAddr]._data)
				return HashAddr;
		}

		//线性探测
		if (ht->_Dete == _IsLine)
		{
			HashAddr = IsLineDete(ht, HashAddr);
			//未找到
			if (HashAddr == starAddr)
				return -1;
		}
		else
			HashAddr = DeteTwo(ht, HashAddr, ++i);
	}

	return -1;
}

void HashTableDestroy(HashTable* ht)
{
	assert(ht);
	free(ht->_table);
	ht->_capacity = 0;
	ht->_size = 0;
	ht->_total = 0;
	ht->_table = NULL;
}

void Test()
{
	HashTable ht;
	HashTableInit(&ht, 17, _IsLine, DataToInt);
	printf("%d\n", HashTableEmpty(&ht));
	HashTableInsert(&ht, 1);
	HashTableInsert(&ht, 11);
	HashTableDelete(&ht, 1);
	HashTableInsert(&ht, 12);
	HashTableInsert(&ht, 33);
	HashTableInsert(&ht, 34);
	printf("%d\n", HashFind(&ht, 34));
	HashTableInsert(&ht, 5);
	HashTableInsert(&ht, 6);
	HashTableInsert(&ht, 7);
	HashTableInsert(&ht, 18);
	HashTableInsert(&ht, 19);
	printf("%d\n", HashTableEmpty(&ht));
	printf("%d\n", HashTableSize(&ht));
	HashTableDestroy(&ht);
}

int main()
{
	Test();
	return 0;
}