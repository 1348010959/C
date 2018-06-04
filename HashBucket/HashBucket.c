#include"HashBucket.h"

//哈希桶初始化
void HashInit(HashBucket* ht, int capacity, PDT _setData)
{
	assert(ht);

	ht->_table = (PNode*)malloc(sizeof(Node)*capacity);
	if (NULL == ht->_table)
		assert(0), exit(1);

	ht->_capacity = capacity;
	ht->_size = 0;
	ht->_setData = _setData;
	for (int i = 0; i < ht->_capacity; ++i)
	{
		ht->_table[i] = NULL;
	}
}

//数据唯一的插入
void HashBuctetInsertUnique(HashBucket* ht, DataType data)
{
	int HashAddr = -1;
	assert(ht);

	int Newdata = ht->_setData(data);
	HashAddr = HashFunc(ht, Newdata);

	PNode pCur = ht->_table[HashAddr];

	while (pCur)
	{
		if (pCur->data == data)
			return;
		else
			pCur = pCur->_pNext;
	}

	pCur = BuyNode(data);

	pCur->_pNext = ht->_table[HashAddr];
	ht->_table[HashAddr] = pCur;
	++ht->_size;
}
//数据唯一的删除
void HashBucketDeleteUnique(HashBucket* ht, DataType data)
{
	assert(ht);
	int HashAddr = -1;
	PNode pPre = NULL;
	PNode pCur = NULL;

	int Newdata = ht->_setData(data);
	HashAddr = HashFunc(ht, Newdata);

	//空桶
	if (NULL == ht->_table[HashAddr])
		return;

	pPre = ht->_table[HashAddr];
	pCur = ht->_table[HashAddr];

	//头结点的值和data相等
	if (pCur->data == data)
	{
		ht->_table[HashAddr] = pCur->_pNext;
		free(pCur);
		--ht->_size;
		return;
	}

	while (pCur)
	{
		if (pCur->data == data)
		{
			break;
		}
		else
		{
			pPre = pCur;
			pCur = pCur->_pNext;
		}
	}

	//判断pCur是否找到了data，退出可能是找到了，或pCur为空
	if (pCur)
	{
		pPre->_pNext = pCur->_pNext;
		free(pCur);
		--ht->_size;
	}
}

//数据不唯一
//数据相同插入
void HashBuctetInsert(HashBucket* ht, DataType data)
{
	int HashAddr = -1;
	assert(ht);

	int Newdata = ht->_setData(data);
	HashAddr = HashFunc(ht, Newdata);

	//头插
	PNode pCur = BuyNode(data);
	pCur->_pNext = ht->_table[HashAddr];
	ht->_table[HashAddr] = pCur;
	ht->_size;
}
//删除所有值为data的元素
void HashBucketDelete(HashBucket* ht, DataType data)
{
	int HashAddr = -1;
	PNode pPre = NULL;
	PNode pCur = NULL;
	assert(ht);

	int Newdata = ht->_setData(data);
	HashAddr = HashFunc(ht, Newdata);

	//空桶
	if (NULL == ht->_table[HashAddr])
		return;

	pPre = ht->_table[HashAddr];
	pCur = ht->_table[HashAddr];

	//data值与哈希中的data相等
	while (pCur&&pCur->data == data)
	{
		ht->_table[HashAddr] = pCur->_pNext;
		free(pCur);
		--ht->_size;

		pCur = ht->_table[HashAddr];
	}

	//后续不为空
	while (pCur)
	{
		if (pCur->data == data)
		{
			pPre->_pNext = pCur->_pNext;
			--ht->_size;
			free(pCur);

			pCur = pPre->_pNext;
		}
		else
		{
			pPre = pCur;
			pCur = pCur->_pNext;
		}
	}
}

//查找
PNode HashBucketFind(HashBucket* ht, DataType data)
{
	int HashAddr = -1;
	assert(ht);

	int Newdata = ht->_setData(data);
	HashAddr = HashFunc(ht, Newdata);

	PNode pCur = ht->_table[HashAddr];
	while (pCur)
	{
		if (pCur->data == data)
			break;
		else
			pCur = pCur->_pNext;
	}

	return pCur;
}

//元素个数
int HashBucketSize(HashBucket* ht)
{
	return ht->_size;
}

//销毁
void DestroyHashBuctet(HashBucket* ht)
{
	assert(ht);

	PNode pDel = NULL;
	int i = 0;
	for (; i < ht->_capacity; ++i)
	{
		pDel = ht->_table[i];
		while (pDel)
		{
			ht->_table[i] = pDel->_pNext;
			free(pDel);
			pDel = ht->_table[i];
		}
	}

	free(ht->_table);
	ht->_size = 0;
	ht->_capacity = 0;
}

//////////////////////////////////////////////

//哈希函数
int HashFunc(HashBucket* ht, int data)
{
	assert(ht);

	return data % (ht->_capacity - 1);
}

//创建新结点
PNode BuyNode(DataType data)
{
	PNode pNewNode = NULL;
	pNewNode = (PNode)malloc(sizeof(Node));
	if (NULL == pNewNode)
		return NULL;

	pNewNode->data = data;
	pNewNode->_pNext = NULL;

	return pNewNode;
}
//打印
void HashBucketPrint(HashBucket* ht)
{
	PNode pCur = NULL;
	assert(ht);
	int i = 0;

	for (; i < ht->_capacity; ++i)
	{
		printf("Hash Bucket%ld:	", i);
		pCur = ht->_table[i];
		while (pCur)
		{
			if (ht->_setData == DataToInt)
			{
				printf("%d\t", pCur->data);
			}
			else
			{
				printf("%s\t", pCur->data);
			}
			pCur = pCur->_pNext;
		}
		printf("\n");
	}
}