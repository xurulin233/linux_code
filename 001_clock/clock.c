#include<stdio.h>
#include <stdlib.h>//用到rand()函数
#include<time.h>   //用到clock()函数
int main() {
	clock_t begintime,endtime;
	int i = 0;
	int a[1002];
	begintime=clock();	//计时开始
	for( i = 1; i <= 1000; i++){//要计时的程序
		a[i] = rand()%200-100;//产生-100到+100之间的随机数
	//printf("  %d",a[i]);
	}
	endtime = clock();	//计时结束
	printf("Running Time：%fs\n", (double)(endtime-begintime)/CLOCKS_PER_SEC);
	return 0;
}
