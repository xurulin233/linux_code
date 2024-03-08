#include <stdio.h>
#include <stdlib.h>


#define STACKINITSIZE 100          //存储空间初始大小
#define STACKINCREMENT 10         //存储空间分配增量

enum return_status
{

    TURE = 1,
    OK = 2,
    FALSE = 0,
    ERROR = -1,
    OVERFLOW = -2,
    STACKEMPTY = -3,
    EMPTY = 1,
    NO_EMPTY = 0
};

typedef struct SqStack {
    int *base;       /*构造之前和销毁之后，base为NULL，用于指示存储数据的数组的地址*/
    int *top;        //栈顶指针
    int stacksize;   //当前已经分配的存储空间大小
}SqStack;

int InitStack(SqStack **s) /*注意：凡是对指针的修改，均需要输入二级指针，这里输入的是指针的地址*/
{
    *s = (SqStack *)malloc(sizeof(SqStack));    /*申请栈指针*/
    if (!(*s))
        exit(OVERFLOW);
    (*s)->top = (*s)->base = (int *)malloc(STACKINITSIZE * sizeof(int)); /*申请内容存储空间*/
    (*s)->stacksize = STACKINITSIZE;
    return OK;
}

int StackEmpty(SqStack *s)
{
    if (s->top == s->base)
        return EMPTY;
    else
        return NO_EMPTY;
}

void Push(SqStack *s, int value)  /*将value压入栈中*/
{
    if (s->top - s->base >= s->stacksize)  /*如果栈满了，追加存储空间*/
    {
        s->base = (int *)realloc(s->base,
            (s->stacksize + STACKINCREMENT) * sizeof(int));
        if (!s->base)
            exit(OVERFLOW);
        s->top = s->base + s->stacksize;    /*重新设置栈顶指针*/
        s->stacksize += STACKINCREMENT;     /*设置栈的存储空间大小*/
    }

    *(s->top++) = value;
}

void Pop(SqStack *s, int *e)
{
    if (StackEmpty(s)) {     /*判断是否栈空*/
        printf("ERROR:当前栈为空！\n");
        exit(STACKEMPTY);
    }
    *e = *(--s->top);       /*top指针减1，然后取出元素*/
}

int StackLength(SqStack *s)
{
    int len = 0;
    int *top, *base;
    top = s->top;
    base = s->base;

    while (top-- != base) /*如果top不等base，则长度+1，且 top--*/
    {
        len++;
    }
    return len;

}

void ClearStack(SqStack *s)
{
    /*直接将栈顶指针指向栈顶，等于栈清空*/
    s->top = s->base;
}

void DestroyStack(SqStack **s )
{
    (*s)->top = NULL;       /*free之前将指针指向NULL，防止野指针*/
    free((*s)->top);
    (*s)->base = NULL;
    free((*s)->base);
    *s = NULL;
    free(*s);
}

int main()
{

    SqStack *s = NULL;

    /*初始化*/
    if (InitStack(&s))
        printf( "堆栈初始化完成！\n" );

    /*入栈*/
    Push(s, 10);
    Push(s, 20);
    Push(s, 30);
    Push(s, 40);

    /*出栈*/
    int val = 0;
    Pop(s, &val);
    printf("val = %d\n", val);
    Pop(s, &val);
    printf("val = %d\n", val);

    /*长度判断*/
    int len = 0;
    len = StackLength(s);
    printf("len = %d\n", len);
    Push(s, 50);
    Push(s, 60);
    Push(s, 70);
    Push(s, 80);
    len = StackLength(s);
    printf("len = %d\n", len);

    /*清空栈*/
    ClearStack(s);
    if (StackEmpty(s))
        printf("当前栈为空！\n");

    /*销毁栈*/
    DestroyStack(&s);

   // system("pause");
    return 0;
}

