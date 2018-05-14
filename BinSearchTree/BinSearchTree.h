#pragma once
#include<stdio.h>
#include<malloc.h>
#include<assert.h>

typedef int DataType;

typedef struct BSTreeNode{
	struct BSTreeNode* _pLeft;
	struct BSTreeNode* _pRight;
	DataType _data;
}BSTNode;

// 初始化二叉搜索树
void InitBSTree(BSTNode** pRoot);

// 插入值为data的元素
void InsertBSTree(BSTNode** pRoot, DataType data);

// 删除值为data的结点
void DeleteBSTree(BSTNode** pRoot, DataType data);

// 在二叉搜索树中查找值为data的结点
BSTNode* FindBSTree(BSTNode* pRoot, DataType data);

// 中序遍历二叉搜索树
void PreOrder(BSTNode* pRoot);

// 销毁二叉搜索树
void DestroyBSTree(BSTNode** pRoot);