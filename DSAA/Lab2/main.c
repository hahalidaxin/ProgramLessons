#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define strLen 12
struct node {
    char data[strLen];
    int depth;
    struct node *fa;
    struct node *lchild;
    struct node *rchild;
};
typedef struct node *TREE;

void pre_order_1(TREE root);
void pre_order_2(TREE root);
void mid_order_1(TREE root);
void mid_order_2(TREE root);
void back_order_1(TREE root);
void back_order_2(TREE root);
void level_order(TREE root);
int check_full(TREE root);
void getfatherAndDepth(TREE a,TREE b,int depth);
void LCA(TREE ra,TREE rb);


//这里注意到并不是所有的树节点的值都是一样的

struct node* stack[MAX];
int stackTop=0;

TREE readTree(TREE root,char* filename) {
    char a[strLen],b[strLen],c[strLen];
    TREE tmp;

    int i,j;
    FILE* fp = fopen(filename,"r");

    fscanf(fp,"%s %s %s",a,b,c);
    root = (TREE) malloc(sizeof(struct node));
    strcpy(root->data,a);
    root->lchild = NULL;
    root->rchild = NULL;
    stack[stackTop++] = root;

    while(fscanf(fp,"%s %s %s",a,b,c)==3) {
        tmp = (TREE) malloc(sizeof(struct node));
        strcpy(tmp->data,a);
        tmp->lchild = NULL;
        tmp->rchild = NULL;
        stack[stackTop++] = tmp;
        if(strcmp(c,"L")==0) {
            for(i=0;i<stackTop;i++) {
                if(strcmp(stack[i]->data,b)==0) {
                    stack[i]->lchild=tmp;
                }
            }
        }   else if(strcmp(c,"R")==0){
            for(i=0;i<stackTop;i++) {
                if(strcmp(stack[i]->data,b)==0) {
                    stack[i]->rchild=tmp;
                }
            }
        }
    }
    return root;
}

int main() {
    TREE tree,ra,rb;
    int i;
    tree = readTree(tree,"ex2_tree3.txt");
    puts("readTree Done!");
    char choose,ta[strLen],tb[strLen];
    while(1) {
        printf("A.pre_order_1  B.pre_order_2  C.mid_order_1  D.mid_order_2  E.back_order_1  F.back_order_2  G.level_order H.LCA\n");
        scanf("%c", &choose);
        if (choose == 'A' )
            pre_order_1(tree);
        else if (choose == 'B' )
            pre_order_2(tree);
        else if (choose == 'C')
            mid_order_1(tree);
        else if (choose == 'D')
            mid_order_2(tree);
        else if (choose == 'E')
            back_order_1(tree);
        else if (choose == 'F')
            back_order_2(tree);
        else if (choose == 'G')
            level_order(tree);
        else if(choose == 'H') {
            puts("input NODE A & NODE B");
            scanf("%s %s",ta,tb);
            for(i=0;i<stackTop;i++) {
                if(strcmp(stack[i]->data,ta)==0) {
                    ra = stack[i];
                    break;
                }
            }
            for(i=0;i<stackTop;i++) {
                if(strcmp(stack[i]->data,tb)==0) {
                    rb = stack[i];
                    break;
                }
            }
            getfatherAndDepth(tree,NULL,0);
            LCA(ra,rb);
        }
        if (check_full(tree) == 1)
            printf("It's a full tree.\n");
        else
            printf("It isn't a full tree.\n");
        getchar();
        printf("\nDo you want to continue? (y/n) \n");
        if (getchar() == 'n')
            break;
        getchar();
    }
    return 0;
}

//递归法
//前序遍历
void pre_order_1(TREE root) {
    if (root != NULL) {
        printf("%s\t", root->data);
        pre_order_1(root->lchild);
        pre_order_1(root->rchild);
    }
}
//非递归法
//前序遍历
void pre_order_2(TREE root) {
    int top = -1;
    TREE str[MAX];              //栈
    while (root != NULL || top != -1) {
        while (root != NULL) {
            printf("%s\t", root->data);
            str[++top] = root;
            root = root->lchild;
        }
        if (top != -1) {
            root = str[top--];
            root = root->rchild;
        }
    }
}

//递归方法
//中序遍历
void mid_order_1(TREE root) {
    if (root != NULL) {
        mid_order_1(root->lchild);
        printf("%s\t", root->data);
        mid_order_1(root->rchild);
    }
}

void mid_order_2(TREE root) {
    int top = -1;
    TREE str[MAX];
    while (root != NULL || top != -1) {
        while (root != NULL) {
            str[++top] = root;
            root = root->lchild;
        }
        if (top != -1) {
            root = str[top--];
            printf("%s\t", root->data);
            root = root->rchild;
        }
    }
}

//后续遍历
//递归做法
void back_order_1(TREE root) {
    if (root != NULL) {
        back_order_1(root->lchild);
        back_order_1(root->rchild);
        printf("%s\t", root->data);
    }
}

//后续遍历
//非递归做法
void back_order_2(TREE root) {
    TREE pre, p;
    int top = -1;
    TREE str[MAX];
    p = root;
    while (p != NULL || top != -1) {
        while (p != NULL) {
            str[++top] = p;
            pre = p->rchild;
            p = p->lchild;
            if (p == NULL)
                p = pre;
        }
        p = str[top--];
        printf("%s\t", p->data);
        if (top != -1 && str[top]->lchild == p)
            p = str[top]->rchild;
        else
            p = NULL;
    }
}

//层序遍历
void level_order(TREE root) {
    int top = 0, tail = 0;
    TREE Q[100];
    if (root == NULL)
        return;
    Q[++tail] = root;
    TREE q;
    q = (TREE) malloc(sizeof(struct node));
    while (top != tail) {           //队列不是空 //前序输出
        q = Q[++top];
        printf("%s\t", q->data);
        if (q->lchild != NULL)
            Q[++tail] = q->lchild;
        if (q->rchild != NULL)
            Q[++tail] = q->rchild;
    }
}

//检查是否是完全二叉树
int check_full(TREE root) {
    int top = 0, tail = 0;
    int s[MAX], i = 0, j = 0;
    TREE Q[MAX];
    if (root == NULL)
        return 1;
    Q[++tail] = root;
    TREE q;
    q = (TREE) malloc(sizeof(struct node));
    while (top != tail) {
        q = Q[++top];

        if (q->lchild != NULL) {
            Q[++tail] = q->lchild;
            s[i++] = 1;
        } else {
            s[i++] = 0;
        }
        if (q->rchild != NULL) {
            Q[++tail] = q->rchild;
            s[i++] = 1;
        } else {
            s[i++] = 0;
        }
    }
    int flag=0;
    for(j=0;j<i;j++) {
        if(!s[j] && flag==0) flag=1;
        if(flag==1 && s[j]==1) return 0;
    }
    return 1;
}

void getfatherAndDepth(TREE root,TREE father,int depth) {
    if(root==NULL) return ;
    root->fa = father;
    root->depth = depth;
    getfatherAndDepth(root->lchild,root,depth+1);
    getfatherAndDepth(root->rchild,root,depth+1);
}

void LCA(TREE a,TREE b) {
    TREE c;
    if(a->depth < b->depth) {
        c = a; a=b; b=c;
    }
    while(a->depth > b->depth) {
        a = a->fa;
    }
    while(a!=b) {
        a = a->fa;
        b = b->fa;
    }
    printf("LCA: %s\n",a->data);
}