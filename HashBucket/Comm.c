#include "Comm.h"

int GetCapacity(size_t capacity)
{
	int i = 0;
	for (; i < 28; i++)
	{
		if (capacity < _PrimeList[i])
			return _PrimeList[i];
	}

	return _PrimeList[27];
}

//将字符串转换为整数
size_t StrToInt(const char* str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}

	return (hash & 0x7FFFFFFF);
}

//整形转无符号整形
size_t DataToInt(int data)
{
	return data;
}
