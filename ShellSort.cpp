#include<iostream>
using namespace std;

void InsertSort(int arr[], int size)
{
	int i = 1;
	//如果只有一个元素，我们认为它有序
	for (; i < size; i++)
	{
		int key = arr[i];
		int end = i - 1;
		while (end >= 0 && key < arr[end])
		{
			arr[end + 1] = arr[end];
			--end;
		}
		arr[end + 1] = key;
	}
}

//二分查找优化
void InsertSort(int arr[], int size)
{
	for (int i = 1; i < size; ++i)
	{
		int key = arr[i];
		int end = i - 1;
		int left = 0;
		int right = i - 1;
		while (left <= right)
		{
			int mid = left + ((right - left) >> 1);
			if (arr[mid] > key)
				right = mid - 1;
			else
				left = mid + 1;
		}

		while (end >= left)
		{
			arr[end + 1] = arr[end];
			--end;
		}
		arr[end + 1] = key;
	}
}

void ShellSort(int arr[], int size)
{
	int gap = size;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (int i = gap; i < size; i += gap)
		{
			int key = arr[i];
			int end = i - gap;

			while (end >= 0 && arr[end] > key)
			{
				arr[end + gap] = arr[end];
				end -= gap;
			}
			arr[end + gap] = key;
		}
	}
}

int main()
{
	int arr[] = { 1, 7, 4, 6, 8, 2, 5, 0 };
	int size = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < size; ++i)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
	ShellSort(arr, size);
	for (int i = 0; i < size; ++i)
	{
		cout << arr[i] << " ";
	}
	return 0;
}
