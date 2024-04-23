#include <stdio.h> //printf
#include <unistd.h> //size_t
#include <stdlib.h> //malloc
#include <time.h>  //time()


void AdjustDown(int* a, int parent, int sz)
{
    int child = parent * 2 + 1;  // 找到父节点的左孩子
    while (child < sz)  // 当左孩子在sz范围内时
    {
        if (child + 1 < sz && a[child] > a[child + 1])  // 如果左孩子比右孩子大
        {
            child++;  // 则将child指向右孩子
        }
        if (a[parent] > a[child])  // 如果父节点比最小的孩子大
        {
            int tmp = a[parent];
            a[parent] = a[child];
            a[child] = tmp;  // 则交换父节点和孩子节点的值
        }
        else  // 如果父节点比最小的孩子小则堆已经建立完成，退出循环
        {
            break;
        }
        parent = child;  // 否则，继续向下调整
        child = parent * 2 + 1;
    }
}

// 打印前k个最大的元素
void PrintTopK(int* a, int n, int k)
{
    // 1. 建堆--用a中前k个元素建堆
    for (int i = (k - 1 - 1) / 2; i >= 0; i--)  // 从数组总长度的一半-1开始，向下调整每一个非叶节点
    {
        AdjustDown(a, i, k);  // 将当前节点向下调整
    }

    // 2. 将剩余n-k个元素依次与堆顶元素比较，如果大于堆顶，则替换堆顶，并向下调整
    for (int i = k; i < n; i++)
    {
        if (a[i] > a[0])  // 如果第i个元素比堆顶元素大
        {
            a[0] = a[i];  // 则将堆顶元素替换为第i个元素
            AdjustDown(a, 0, k);  // 向下调整堆顶元素
        }
    }

    // 3. 打印前k个元素
    for (int i = 0; i < k; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

// 测试函数
void TestTopk()
{
    int n = 10000;
    int* a = (int*)malloc(sizeof(int) * n);
    srand(time(0));

    // 生成数组
    for (size_t i = 0; i < n; ++i)
    {
        a[i] = rand() % 1000000;
    }

    // 修改一些元素，这些元素就是最大的.
    a[5] = 1000000 + 1;
    a[1231] = 1000000 + 2;
    a[531] = 1000000 + 3;
    a[5121] = 1000000 + 4;
    a[115] = 1000000 + 5;
    a[2335] = 1000000 + 6;
    a[9999] = 1000000 + 7;
    a[76] = 1000000 + 8;
    a[423] = 1000000 + 9;
    a[3144] = 1000000 + 10;

    // 打印前10个最大的元素
    PrintTopK(a, n, 10);
}


int main()
{

	TestTopk();
	return 0;
}



