#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<assert.h>

#define size_t unsigned long

typedef char* DataType;

typedef size_t(*PDT)(DataType data);

typedef struct Node
{
	struct  Node* _pNext;
	DataType data;
}Node,*PNode;

//哈希表
typedef struct HashBucket
{
	PNode* _table;
	int _capacity;
	int _size;
	PDT _setData;
}HashBucket;

//哈希桶初始化
void HashInit(HashBucket* ht, int capacity, PDT _setData);

//数据唯一的插入与删除
void HashBuctetInsertUnique(HashBucket* ht, DataType data);
void HashBucketDeleteUnique(HashBucket* ht, DataType data);

//数据不唯一
void HashBuctetInsert(HashBucket* ht, DataType data);
void HashBucketDelete(HashBucket* ht, DataType data);

//查找
PNode HashBucketFind(HashBucket* ht, DataType data);

//元素个数
int HashBucketSize(HashBucket* ht);

//销毁
void DestroyHashBuctet(HashBucket* ht);

//////////////////////////////////////////////

//哈希函数
int HashFunc(HashBucket* ht, int data);

//创建新结点
PNode BuyNode(DataType data);
//打印
void HashBucketPrint(HashBucket* ht);

size_t StrToInt(const char* str);

size_t DataToInt(int data);

