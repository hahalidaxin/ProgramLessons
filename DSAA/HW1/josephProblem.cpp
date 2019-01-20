#include<iostream>
#include <cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
using namespace std;

typedef struct node
{
	int id;
	struct node *front,*next;
}Node,*PNode;
int m,n;

PNode Create_list()
{
	PNode head,p,q;
	head=(Node *)malloc(sizeof(Node));
	head->id=1;
	p=(Node *)malloc(sizeof(Node));
	p=head;
	for(int i=2;i<=n;i++)
	{
		q=(Node *)malloc(sizeof(Node));
		q->id=i;
		q->front=p;
		p->next=q;
		p=q;
	}
	p->next=head;head->front=p;
	return head;
}

void Delete_List(PNode p)
{
	while(p->next){
		PNode q;
		q=p->next;
		free(p);
		p=q;
	}
}
PNode Get_Next_Node(PNode head,int k)
{
	PNode p=head;
	for(int i=1;i<k;i++)
	{
	  p=p->next;
	}
	return p;
}
PNode Get_front_Node(PNode head,int k)
{
	PNode p=head;
	for(int i=1;i<k;i++)
	{
		p=p->front;
	}
	return p;
}

void Delete_Node(PNode p)
{
	PNode q;
	(p->front)->next=p->next;
	(p->next)->front=p->front;
}

void Print_List(PNode head)
{
	PNode p=head;
	printf("%d ",p->id);
	p=p->next;
	while(1){
	   if(p==head) break;
	   printf("%d ",p->id);
	   p=p->next;
	}
	printf("\n");
}
int main()
{
    cin>>n>>m;
    PNode head,p,q;
    head=Create_list();
	int ans;p=head;
     while(1){
		p=Get_Next_Node(p,m);
		q=p->next;ans=p->id;
	    if(p->next==p) break;
	    Delete_Node(p);
	    p=q;
     }
     printf("%d\n",ans);
    return 0;
}