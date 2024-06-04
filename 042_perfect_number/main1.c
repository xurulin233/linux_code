#include<stdio.h>
 
int main()
{
    int i,j,k=0,a[1000],t,n;
    printf("请输入数字n:");
    scanf("%d",&n);
    for(i=2;i<=n;i++)
        {
          t=i;
          for(j=1;j<i;j++)
            {
                if(i%j==0)
                t-=j;
            }
            if(t==0)
            {
                a[k]=i;
                k++;
            }
        }
    if(k!=0)
    {
         for(i=0;i<k;i++)
            {
              printf("%d ",a[i]);
            }
    }
   else
   {
       printf("No!");
 
   }
 
    return 0;
}
