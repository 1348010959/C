#ifndef __GAME_H__
#define __GAME_H__

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define ROW 9
#define COL 9

#define ROWS ROW+2
#define COLS COL+2

#define EASY_COUNT 10

void Init(char board[ROWS][COLS], int rows, int cols,char set);
void Display(char board[ROWS][COLS], int rows, int cols);
void Setmine(char board[ROWS][COLS], int row, int col);
int Getminecount(char board[ROWS][COLS], int row, int col);
#endif //__GAME_H__


#define _CRT_SECURE_NO_WARNINGS 1
#include"game.h"

//初始化雷盘
void Init(char board[ROWS][COLS], int rows, int cols,char set)
{
	int i = 0;
	int j = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			board[i][j] = set;
		}
	}
}

//打印雷盘
void Display(char board[ROWS][COLS], int rows, int cols)
{
	int i = 0;
	//列号
	printf("  ");
	for (i = 1; i <= rows; i++)
	{
		printf("%d ", i);
	}
	printf("\n");
	for (i = 1; i <= rows; i++)
	{
		printf("%d ", i);
		int j = 0;
		for (j = 1; j <= cols; j++)
		{
			printf("%c ", board[i][j]);
		}
		printf("\n");
	}
}

//布雷
void Setmine(char board[ROWS][COLS], int row, int col,int count)
{
	while (count)
	{
		int x = rand() % 9 + 1;
		int y = rand() % 9 + 1;
		if (board[x][y] == '0')
		{
			board[x][y] = '1';
			count--;
		}
	}
}

int Getminecount(char board[ROWS][COLS], int x, int y)
{
	return board[x - 1][y] +
		board[x - 1][y - 1] +
		board[x][y - 1] +
		board[x + 1][y - 1] +
		board[x + 1][y] +
		board[x + 1][y + 1] +
		board[x][y + 1] +
		board[x - 1][y + 1] - 8 * '0';
}
