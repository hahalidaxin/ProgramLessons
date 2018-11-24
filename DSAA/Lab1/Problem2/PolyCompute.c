//
// Created by DaxinLi on 2018/11/10.
//
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

typedef struct node{
    int coef;
    int exp;
    struct node* next;
 }pol,*Pol;

//按照降序在指定位置插入指定的节点
Pol InsertNode(Pol head,int coef,int exp)
{
    Pol pNow = head;
    if(exp>head->exp) {
        Pol newNode = (Pol)malloc(sizeof(struct node));
        newNode->coef = coef;
        newNode->exp = exp;
        newNode->next = head;
        head = newNode;
        return head;
    }
    while(pNow!=NULL) {
        int left = pNow->exp;
        int right = pNow->next==NULL? -(1<<30):pNow->next->exp;
        if(exp<=left && exp>=right) {
            Pol newNode = (Pol )malloc(sizeof(struct node));
            newNode->coef = coef;
            newNode->exp = exp;
            newNode->next = pNow->next;
            pNow->next = newNode;
            return head;
        }
        pNow = pNow->next;
    }
    return head;
}
Pol CreateList(int coef,int exp)
{
    Pol pHead = (Pol)malloc(sizeof(struct node));
    Pol pTail = pHead;
    pTail->next = NULL;
    pTail->coef = coef;
    pTail->exp = exp;
    return pTail;
}

Pol add(Pol a1,Pol b1){
    Pol a,b,c;
    int sumc,coef,exp;;
    a = a1;
    b = b1;
    c = NULL;
    while(a!=NULL && b!=NULL)
    {
        if(a->exp > b->exp)
        {
            coef = a->coef;
            exp = a->exp;
            a = a->next;
        }
        else if(a->exp < b->exp)
        {
            coef = b->coef;
            exp = b->exp;
            b = b->next;
        }
        else
        {
           sumc = a->coef + b->coef;
           coef = sumc;
           exp = a->exp;
           a = a->next;
           b = b->next;
        }
        if(coef!=0) {
            if(c==NULL) {
                c = CreateList(coef,exp);
            } else {
                c = InsertNode(c,coef,exp);
            }
        }
    }
    coef = 0;
    while(a != NULL)
    {
        coef = a->coef;
        exp = a->exp;
        if(c==NULL) {
            c = CreateList(coef,exp);
        } else {
            c = InsertNode(c,coef,exp);
        }
        a = a->next;
    }
    while(b != NULL)
    {
        coef = b->coef;
        exp = b->exp;
        if(c==NULL) {
            c = CreateList(coef,exp);
        } else {
            c = InsertNode(c,coef,exp);
        }
        b = b->next;
    }
    return c;
}

void printPolyment(Pol head) {
    Pol pre = head;
    while(pre!=NULL) {
        printf("[%d %d] ",pre->coef,pre->exp);
        pre = pre->next;
    }
    puts("");
}

void PolyComputeMain() {
    puts("yes");
    FILE *fp = fopen("../Problem2/input.txt","r");
    if(fp==NULL) {
        puts("file not found");
        return ;
    }
    int n;
    fscanf(fp,"%d",&n);
    while(n--) {
        Pol polia = NULL;
        Pol polib = NULL;
        int coef,exp;
        while(1) {
            fscanf(fp,"%d%d", &coef, &exp);
            if (exp < 0) break;
            if(polia==NULL) {
                polia = CreateList(coef,exp);
            } else {
                polia = InsertNode(polia, coef, exp);
            }
        }
        puts("polia");
        printPolyment(polia);
        while(1) {
            fscanf(fp,"%d%d", &coef, &exp);
            if (exp < 0) break;
            if(polib==NULL) {
                polib = CreateList(coef,exp);
            } else {
                polib = InsertNode(polib, coef, exp);
            }
        }
        puts("polib");
        printPolyment(polib);
        puts("polians");
        Pol polians = add(polia,polib);
        printPolyment(polians);
    }
}

int main() {
    PolyComputeMain();
}