//
// Created by DaxinLi on 2018/11/10.
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include<string.h>

typedef struct node
{
    int data;
    struct node *pNext;
} List;


List* CreateList(int x)
{
    List* pHead = (List*)malloc(sizeof(List));
    List* pTail = pHead;
    pTail->pNext = NULL;
    pTail->data = x;
    return pTail;
}


int Length(List* list)
{
    List *p =list;
    int i=0;
    while (p->pNext)
    {
        p=p->pNext;
        i++;
    }
    return i;
}
List* Findnth(List* list,int posit)
{
    List* p=list;
    int i=1;
    p=p->pNext;
    while (p!=NULL&&i<posit)
    {
        p=p->pNext;
        i++;
    }
    if (i==posit)
        return p;
    else
        return NULL;
}


//按照升序在指定位置插入指定的节点
List* InsertNode(List* head,int data)
{
    List*pNow = head;
    if(data < head->data) {
        List* newNode = (List*)malloc(sizeof(List));
        newNode->data = data;
        newNode->pNext = head;
        head = newNode;
        return head;
    }
    while(pNow!=NULL) {
        int left = pNow->data;
        int right = pNow->pNext==NULL? (1<<30):pNow->pNext->data;
        if(data>=left && data<=right) {
            List*newNode = (List*)malloc(sizeof(List));
            newNode->data = data;
            newNode->pNext = pNow->pNext;
            pNow->pNext = newNode;
            return head;
        }
        pNow = pNow->pNext;
    }
    return head;
}
//向nowP指针之后插入一个指定的值，返回插入元素的指针
List* InsertRight(List* nowP,int x) {
    List* pNew = (List*)malloc(sizeof(List));
    pNew->pNext = nowP->pNext;
    pNew->data = x;
    nowP->pNext = pNew;
    return pNew;
}

//指定函数
List* findFixedNode(List* head, int x) {
    List* pNow = head;
    while(pNow!=NULL) {
        if(pNow->data==x) {
            return pNow;
        }
        pNow = pNow->pNext;
    }
    return NULL;
}
//删除一个指定值为data的元素
//返回头指针
List* DelNode(List* head,int data)
{
    List* pNow = head;
    if(head->data==data) {
        pNow = head;
        head = pNow->pNext;
        free(pNow);
        return head;
    }
    while(pNow->pNext != NULL) {
        if(pNow->pNext->data==data) {
            List* ptmp = pNow->pNext;
            pNow->pNext = pNow->pNext->pNext;
            free(ptmp);
            return head;
        }
        pNow = pNow->pNext;
    }
    return NULL;
}

void printAll(List* head) {
    List* pNow = head;
    while(pNow!=NULL) {
        printf("%d ",pNow->data);
        pNow = pNow->pNext;
    }
    puts("");
}
int getLenghtofList(List* head) {
    int cnt = 0;
    List* pNow = head;
    while(pNow!=NULL) {
        cnt+=1;
        pNow=pNow->pNext;
    }
    return cnt;
}
List* getInversedList(List* head) {
    int inputList[100],cnt=0;
    List* pNow = head;
    while(pNow!=NULL) {
        inputList[cnt++] = pNow->data;
        pNow = pNow->pNext;
    }
    List* newHead = CreateList(inputList[--cnt]);
    pNow = newHead;
    while(cnt) {
        pNow = InsertRight(pNow,inputList[--cnt]);
    }
    return newHead;
}
void LinearListMain() {
    List* head = NULL;
    List* tail = NULL;
    List* nowP = head;
    int x;
//    读入数据 构造链表
    FILE* fp = fopen("../Problem1/input.txt","r");
    if(fp==NULL) {
        printf("File not found!");
        return ;
    }
    puts("readFile Done");

    int input[20], cnt;
    while (!feof(fp)) {
        fscanf(fp, "%d", &x);
        if (nowP == NULL) {
            head = CreateList(x);
            nowP = head;
        } else {
            head = InsertNode(head, x);
        }
    }
    tail = nowP;

    while(1) {
        puts("LinearList System:");
        puts("1.List All LinearList");
        puts("2.Insert a Node");
        puts("3.Delete a Node");
        puts("4.get Length of LinearList");
        puts("5.Inverse the LinearList");
        puts("0.Exit");
        int op;
        scanf("%d",&op);
        switch(op) {
            case 0:
                return ;
                break;
            case 1:
                printAll(head);
                break;
            case 2:
                puts("input the data x");
                scanf("%d",&x);
                if(head==NULL) {
                    head = CreateList(x);
                } else {
                    head  = InsertNode(head,x);
                }
                printAll(head);
                break;
            case 3:
                puts("delete the data x");
                scanf("%d",&x);
                List *ans = DelNode(head, x);
                if (ans == NULL) {
                    puts("not found");
                } else {
                    printAll(ans);
                    head = ans;
                }
                break;
            case 4:
                printf("%d\n", getLenghtofList(head));
                break;
            case 5:
                head = getInversedList(head);
                printAll(head);
                break;
        }
    }
}

int main()
{
    LinearListMain();
}