#include <stdio.h>
#include <stdlib.h>
 
int main()
{
    int n;
    int i,j,sum,k,b[1000],w[1000],s=0,m;
    printf("请输入数字n:");
    scanf("%d",&n);
   for(i=n;i>0;i--)
   {
       sum=0,k=0;
       for(j=1;j<i;j++)
        {
            if(i%j==0)
            {
                b[k]=j;
                k++;
            }
        }
       for(m=0;m<k;m++)
       {
           sum+=b[m];
       }
       if(sum==i)
       {
           w[s]=i;
           s++;
       }
   }
     if(s!=0)
     {
        for(i=0;i<s;i++)
        printf("%d ",w[i]);
     }
     else
     {
         printf("No!");
     }
 
 
    return 0;
}
