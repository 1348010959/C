#include<stdio.h>
#include<malloc.h>

int *BuyZone(int n)
{
	int* x = (int*)malloc(sizeof(int)* n);
	return x;
}

int main()
{
	int n = 0;
	int *x = NULL;
	int i = 0;
	int tmp = 0;
	int max = 0;
	int next = 0;
	scanf("%d", &n);
	x = BuyZone(n);
	for (; i < n; ++i)
	{
		scanf("%d", &tmp);
		*(x + i) = tmp;
	}
	if (n < 3)
	{
		max = x[0] + x[1];
	}
	for (i = 0; i < n - 2; ++i)
	{
		max = *(x + i) + *(x + i + 1);
		next = *(x + i + 2) + *(x + i + 1);
		if (max < next)
			max = next;
	}
	printf("max = %d", max);
	return 0;
}