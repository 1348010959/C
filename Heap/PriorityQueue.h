#include"May_12.h"
typedef struct PriorityQueue
{
	Heap _hp;
}PriorityQueue;

// 优先级队列初始化
void PriorityQueueInit(PriorityQueue* q, Compare com);

// 向队列中插入元素
void PriorityQueuePush(PriorityQueue* q, DataType data);

// 删除优先级最高的元素
void PriorityQueuePop(PriorityQueue* q);

// 获取队列中优先级最高的元素
int PriorityQueueSize(PriorityQueue* q);

// 检测优先级队列是否为空
int PriorityQueueEmpty(PriorityQueue* q);

// 获取堆顶的元素
DataType PriorityQueueTop(PriorityQueue* q);

// 销毁优先级队列
void PriorityQueueDestroy(PriorityQueue* q);