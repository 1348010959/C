//////////////////////////////////////////////////////////////////////////////
//Stack.h

#pragma once
#include<assert.h>
#include"Maze.h"

#define MAX_SIZE 100

extern Pos;

typedef Pos DataType;

typedef struct Stack
{
	DataType  _data[MAX_SIZE];
	int _top;
}SqStack;

void SqStackInit(SqStack* Stack);    //初始化

void Push(SqStack* Stack, DataType data);   //压栈

void Pop(SqStack* Stack);    //弹栈

Pos StackTop(SqStack*Stack);    //栈顶元素个数

int StackTopMaxSize();    //栈能容纳的元素个数

/////////////////////////////////////////////////////////////////////////////////
//Maze.h
#pragma once
#include<stdio.h>
#include<assert.h>

#define N 6
typedef struct Pos{
	int _x; //行
	int _y; //列
}Pos;

typedef struct Maze
{
	int _mz[N][N];
	Pos _entry;
}Maze;
void MazeInit(Maze* m, int arr[][N]);    //初始化

void MazeGetPath(Maze* m);  //寻找路径

void MazeGetShortPath(Maze* m); //最优路径

void MazePrint(Maze* m);    //打印迷宫

int MazeEntry(Pos entry);	//判断迷宫入口是否正确

////////////////////////////////////////////////////////////////////////////////
//Stack.c

#include"Stack.h"                                                                                                                                                                                    
#include"Maze.h"

 void SqStackInit(SqStack* Stack)
{
	     assert(Stack);
	
		     Stack->_top = 0;
	 }

 void Push(SqStack*Stack, DataType data)
 {
	     assert(Stack);
	
		     if (Stack->_top == MAX_SIZE)
		     {
		         printf("栈已满!!!\n");
		         return;
		     }
	     Stack->_data[Stack->_top] = data;
	     ++Stack->_top;
	 }

 void Pop(SqStack*Stack)
 {
     assert(Stack);
	
		     if (Stack->_top == 0)
	     {
		        printf("栈已空\n");
		         return;
		     }
	     --Stack->_top;
	 }

 Pos StackTop(SqStack* Stack)
 {
	     assert(Stack);
	
		 return Stack->_data[Stack->_top - 1];
	 }

 int StackTopMaxSize()
 {
	     return MAX_SIZE;
 }
 
 
 //////////////////////////////////////////////////////////////////////////////////
 //MazeFunction.c
 
                                                                                                                                                                                     buffers
#include"Maze.h"
#include"Stack.h"

 void MazeInit(Maze*m, int arr[][N])
 {
     int i = 0;
	 int j = 0;
	 assert(m);

	 for (; i < N; ++i)
	 {
		 for (j = 0; j<N; ++j)
		 {
			 m->_mz[i][j] = arr[i][j];
		 }
	 }
 }

 void MazeGetPath(Maze* m)
 {
	 m->_entry._x = 5;
	 m->_entry._y = 2;
	 SqStack s;
	 SqStackInit(&s);
	 Push(&s, m->_entry);
	 while (s._top)
	 {
		 Pos cur, next;
		 cur = StackTop(&s);
		 m->_mz[cur._x][cur._y] = 2;

		 if ((cur._x == 0 || cur._x == N - 1 || \
			 cur._y == 0 || cur._y == N - 1) && (cur._x != m->_entry._x && cur._y != m->_entry._y))
		 {
			 printf("找到出口！！！\n");
			 return;
		 }
		 
		 next = cur;
		 //左
		 next._x -= 1;
		 if (CheckIsAccess(m, next) == 1)
		 {
			 Push(&s, next);
			 continue;
		 }

		 next = cur;
		 //上
		 next._y -= 1;
		 if (CheckIsAccess(m, next) == 1)
		 {
			 Push(&s, next);
			 continue;
		 }

		 next = cur;
		 //右
		 next._x += 1;
		 if (CheckIsAccess(m, next) == 1)
		 {
			 Push(&s, next);
			 continue;
		 }

		 next = cur;
		 //下
		 next._y += 1;
		 if (CheckIsAccess(m, next) == 1)
		 {
			 Push(&s, next);
			 continue;
		 }

		 //回溯
		 Pop(&s);
	 }
	 printf("这迷宫有毒，跑不出去~\n");
 }

 //判断迷宫入口是否正确
 int MazeEntry(Pos entry)
 {
	 if (entry._x == 0 || entry._x == N-1 || \
		 entry._y == 0 || entry._y == N-1)
		 return 1;
	 return 0;
 }

 //判断下一步是否正确
int CheckIsAccess(Maze* m, Pos next)
 {
	 assert(m);

	 if (next._x >= 0 && next._x<N && next._y>=0 && next._y < N\
		 &&m->_mz[next._x][next._y] == 1)
		 return 1;
	 return 0;
 }

 void MazePrint(Maze* m)
 {
	 int i = 0;
	 int j = 0;
	 assert(m);

	 for (; i<N; ++i)
	 {
		 for (j = 0; j<N; ++j)
		 {
			 printf("%d ", m->_mz[i][j]);
		 }
		 printf("\n");
	 }
 }

 
 ////////////////////////////////////////////////////////////
 //Maze.c
 
 
#include"Maze.h"                                                            

 int main()
 {
	 Maze m;
	 int a[N][N] = {
		                 {0, 0, 0, 0, 0, 0},
		                 {0, 0, 1, 0, 0, 0},
		                 {0, 0, 1, 0, 0, 0},
		                 {0, 0, 1, 1, 1, 0},
		                 {0, 0, 1, 0, 1, 1},
		                 {0, 0, 1, 0, 0, 0}
				   };
     MazeInit(&m, a);
	 MazePrint(&m);
	 MazeGetPath(&m);
	 MazePrint(&m);
	 return 0;
 }
