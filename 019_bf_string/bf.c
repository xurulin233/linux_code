#include<stdio.h>
#include<assert.h>
#include<string.h>
 
int BF(char* str, char* sub)//str代表主串，sub代表子串
{
	assert(str&&sub);//断言
	if (str == NULL || sub == NULL)//串为空值时直接返回-1
	{
		return -1;
	}
	int i = 0;//遍历主串
	int j = 0;//遍历子串
	int lenstr = strlen(str);//求出主串的长度
	int lensub = strlen(sub);//求出子串的长度
	while ((i < lenstr) && (j < lensub))//当子串遍历结束或主串遍历结束时，跳出循环
	{
		if (str[i] == sub[j])//匹配成功
		{
			i++;
			j++;
		}
		else//匹配失败
		{
			i = i - j + 1;
			j = 0;
		}
	}
	if (j >= lensub)//如果是因为子串遍历结束而跳出循环，说明匹配成功，返回下标
	{
		return i - j;
	}
	else//匹配失败，返回-1
		return -1;
}
int main()
{
	printf("%d\n", BF("ababcabcdabcde", "abcd"));//匹配成功，预期返回下标5
	printf("%d\n", BF("ababcabcdabcde", "abcds"));//匹配失败，返回-1
	printf("%d\n", BF("ababcabcdabcde", "ab"));//匹配成功，返回下标0
 
	return 0;
}

