#pragma once
#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include"Comm.h"

typedef int DataType;

typedef enum State
{
	EMPTY,	//当前位置为空
	EXIST,	//当前位置有元素
	DELETE	//当前位置被删除
}State;

//元素
typedef struct Elem
{
	DataType _data;
	State _state;
}Elem;

typedef enum
{
	_IsLine,	//线性探测
	_IsNotLine	//二次探测
}IsLine;

typedef size_t(*PDT)(DataType str);

typedef struct HashTable
{
	Elem* _table;
	int _capacity;	//容量
	int _size;		//存在位元素个数
	int _total;		//存在位和删除位总共的个数
	IsLine _Dete;	//探测方法
	PDT _setData;	//将数据转换为类型
}HashTable;

void HashTableInit(HashTable* ht,int capacity, IsLine _Dete, PDT setData); 
void HashTableInsert(HashTable* ht,DataType data); 
void HashTableDelete(HashTable* ht, DataType data); 
int HashTableSize(HashTable* ht);
int HashTableEmpty(HashTable* ht);
void HashTableDestroy(HashTable* ht);

int HashFind(HashTable* ht, DataType data);
