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

//#include<stdio.h>
//
//struct celltype {
//	int x;
//	celltype *next,*prev;
//};
//
//void Delete(celltype* p) {
//	if(p->prev != NULl) {
//		p->prev->next = p->next;
//	} 
//	if(p->next != NULL) {
//		p->next->prev = p->prev;
//	}
//	delete p;
//}
//void rightInsert(celltype* p,int x) {
//	celltype xp = new Node;
//	xp->x = x;
//	if(p!=NULL) {
//		xp->next = p->next;
//	}
//	xp->prev = p;
//	if(p->next != NULL && p->next->prev != NULL) {
//		p->next->prev = xp;
//	}
//	p->next = xp;
//}
//
//int JosephMain(int n,int m) {
//	celltype* head = null;
//	rightInsert(head,1);
//	celltype* pnow = head;
//	for (int i=2;i<=n;i+=1) {
//		rightInsert(pnow,i);
//		pnow = pnow->next;
//	}
//	pnow->next = head;
//	head->prev = pnow;
//	
//	int killedCount = 0;
//	int roundCount = 0;
//	pnow = head;
//	while(true) {
//		if(killedCount==n-1) {
//			int ans = pnow->x;
//			Delete(pnow);
//			return ans;
//		}
//		roundCount+=1;
//		if(roundCount==m) {
//			Delete(pnow);
//			roundCount=0;
//		}
//		pnow = pnow->next;
//	}
//}
//
//int main() {
//	printf("%d",JosephMain(37,3));
//	return 0;
//} 

