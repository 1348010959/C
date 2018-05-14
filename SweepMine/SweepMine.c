#define _CRT_SECURE_NO_WARNINGS 1
#include"game.h"

void Menu()
{
	printf("*********************************************\n");
	printf("********* 0.exit             1.play *********\n");
	printf("*********************************************\n");
}

void Game()
{
	char mine[ROWS][COLS];
	char show[ROWS][COLS];
	int x = 0;
	int y = 0;
	int win = 0;
	Init(mine, ROWS, COLS,'0');
	Init(show, ROWS, COLS,'*');

	Setmine(mine,ROW,COL,EASY_COUNT);
	Display(mine, ROW, COL);	//真实雷盘
	Display(show, ROW, COL);	//对外显示的上层虚拟雷盘

	while (win<ROW*COL-EASY_COUNT)
	{
		printf("请输入坐标\n");
		scanf("%d%d", &x, &y);
		if (x >= 1 && x <= 9 && y >= 1 && y <= 9)
		{
			if ((win == 0) && mine[x][y] == '1')
			{
				mine[x][y] = '0';
				Setmine(mine, ROW, COL, 1);
				//Display(mine, ROW, COL);
			}
			if (mine[x][y] == '0')
			{
				//统计雷的个数
				int count = Getminecount(mine, x, y);
				show[x][y] = count + '0';
				Display(show, ROW, COL);
				win++;
			}
			else
			{
				printf("很遗憾，你被炸死了\n");
				break;
			}
		}
		else
		{
			printf("坐标非法\n");
		}
	}
	if (win == ROW*COL - EASY_COUNT)
	{
		printf("恭喜你，排雷成功\n");
	}
	Display(mine, ROW, COL);
}

void test()
{
	int input = 0;
	do{
		Menu();
		printf("请选择>:");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			printf("开始游戏\n");
			Game();
			break;
		case 0:
			printf("退出游戏\n");
			break;
		default:
			break;
		}
	} while (input);
}

int main()
{
	test();
	system("pause");
	return 0;
}