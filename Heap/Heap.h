#pragma once
#include<stdio.h>
#include<malloc.h>
#include<assert.h>

typedef int DataType;

typedef int (*Compare)(DataType x, DataType y);

typedef struct Heap
{
	DataType* _data;
	int _size;
	int _capacity;
	Compare comp;
}Heap;

//初始化
void HeapInit(Heap* hp, Compare cmp);

//创建堆
void CreteHeap(Heap* hp, int arr[], int size);

//插入元素
void InsertHeap(Heap* hp, DataType data);

//返回堆顶元素
DataType HeapTop(Heap* hp);

// 检测一个堆是否为空堆
int EmptyHeap(Heap* hp);

// 获取堆中元素的个数
int SizeHeap(Heap* hp);

// 删除堆顶元素
void DeleteHeap(Heap* hp);

// 销毁堆
void DestroyHeap(Heap* hp);

