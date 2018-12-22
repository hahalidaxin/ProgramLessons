#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 1024


typedef struct node {
    int data;
    struct node* lch;
    struct node* rch;
} BSTnode;
typedef BSTnode* BST;

void GenData() {        //生成数据
    FILE *fp1,*fp2;
    int i,w,temp,j;
    int a[N];
    fp1=fopen("sorted.txt","w");
    for(i=0;i<N;i++) {
        j=2*i+1;
        fprintf(fp1,"%d\n",j);
    }
    fclose(fp1);
    fp2=fopen("random.txt","w");
    for(i=0;i<N;i++){
        a[i]=2*i+1;
    }
    srand((int)(time(0)));
    for(i=0;i<N;i++) {      //通过交换产生随机序列
        w=rand() % N;
        temp=a[i];
        a[i]=a[w];
        a[w]=temp;
    }
    for(i=0;i<N;i++){
        fprintf(fp2,"%d\n",a[i]);
    }
    fclose(fp2);
    printf("write into random.txt sorted.txt SUCESSFULLY\n");
}


void Insert(int key,BST *T) {       //插入节点
    if(*T==NULL) {
        *T=(BST)malloc(sizeof(BSTnode));
        (*T)->data=key;
        (*T)->lch=(*T)->rch=NULL;
    }
    else if((*T)->data>key){
        Insert(key,&((*T)->lch));
    }
    else if((*T)->data<key){
        Insert(key,&((*T)->rch));
    }
    else if((*T)->data==key) return;
}

BST CreateBST(FILE* fp){    //建树
    int i,data;
    BST T=NULL;
    for(i=0;i<N;i++){
        fscanf(fp,"%d",&data);
        Insert(data,&T);
    }
    return T;
}

int cnt=0;
int Find(int key,BST T) {        //BST查找算法
    if(T==NULL) {
        return 0;
    }
    cnt++;
    if(T->data==key) {
        return 1;
    }
    else if(T->data>key) {
        Find(key,T->lch);
    }
    else if(T->data<key){
        Find(key,T->rch);
    }
}


int deletemin(BST *T){  //选择最小关键字 该关键字大于删除节点左子树所有节点 小于右子树所有节点
    int temp;
    BST p;
    if((*T)->lch==NULL) {   //左子树是空的，当前节点就是最小节点
        p=(*T);
        temp=p->data;
        (*T)=(*T)->rch;
        return temp;
    }
    else return deletemin(&((*T)->lch));    //左子树不空 最小节点在左子树中
}

void DeleteB(int key,BST *T){   //节点删除
    if((*T)!=NULL){
        if(key < (*T)->data)
            DeleteB(key,&((*T)->lch));
        else if(key > (*T)->data)
            DeleteB(key,&((*T)->rch));
        else if(key==(*T)->data){
            if((*T)->rch==NULL)
                (*T)=(*T)->lch;
            else if((*T)->lch==NULL)
                (*T)=(*T)->rch;
            else{
                (*T)->data=deletemin(&((*T)->rch));     //用右子树最小节点来替代当前节点
            }
        }
    }
}

void Test(BST T){       //检测二叉树性能的函数
    puts("------- TEST BST START-------");
    int i,sum=0;        //用循环遍历的方式计算有序/无序建树后的平均查找次数
    float ans;
    for(i=0;i<N;i++) {
        cnt=0;
        Find(2*i+1,T);
        sum += cnt;
    }
    ans=(float)sum/N;
    printf("Cond [FIND] average count：%f\n",ans);
    sum=0;     ///重新置为0
    for(i=0;i<N;i++){
        cnt=0;
        Find(2*i,T);
        sum += cnt;
    }
    ans=(float)sum/N;
    printf("Cond [UNFIND] average count: %f\n",ans);
    puts("------- TEST BST END-------");
}

int anscnt=0;
void Inorder(BST T,int *inorderData){      ///通过中序编列得到BST的有序序列
    if(T==NULL){
        return;
    }
    Inorder(T->lch,inorderData);
    inorderData[anscnt++] = T->data;
    Inorder(T->rch,inorderData);
}

int inorder[N];
void BST2InorderData(BST T){     //BST Create Inorder Data
    anscnt = 0;
    Inorder(T,inorder);
}

int half_cnt;
int Half_Search(int key){           //二分查找
    int mid,L,R;
    L=0;
    R=N-1;
//    for(int i=0;i<N;i++)
//        printf("%d ",inorder[i]);
//    puts("");
//    puts("yes");
    while(L<=R) {
        half_cnt++;
        mid=(L+R)/2;
        if(inorder[mid]==key) { return 1; }
        else if(key < inorder[mid]) R=mid-1;
        else L=mid+1 ;
    }
    return 0;

}
int Test_half() {
    puts("------- TEST HALF_SEARCH START-------");
    int i,sum=0;
    float aver;
    for(i=0;i<N;i++){
        half_cnt=0;
        Half_Search(2*i+1);
        sum+=half_cnt;
    }
    aver=(float)sum/N;
    printf("Cond. [FIND] average count：%f\n",aver);
    sum=0;     ///重新置为0
    for(i=0;i<N;i++){
        half_cnt=0;
        Half_Search(2*i);
        sum+=half_cnt;
    }
    aver=(float)sum/N;
    printf("Cond. [UNFIND] average count：%f\n",aver);
    puts("------- TEST HALF_SEARCH START-------");
}

void manu(){
    printf("[1].BST Search [Sorted Order Create]\n");
    printf("[2].BST Search [Unsorted Order Create]\n");
    printf("[3].BST Delete for Both\n");
    printf("[4].Test BST (2 Cond. AVER CMP)\n");       //两种查询方式查询（成功不成功） 平均查找长度
    printf("[5].Half_Search\n");
    printf("[6].Test Half_Search (2 Cond. AVER CMP)\n");   //两种查询方式查询（成功不成功） 平均查找长度
    puts("[7].BST:Sort");
    printf("[0].exit\n");
}
int main()
{
    BST Ts,Tr;
    FILE *fp1,*fp2;
    int key,ch,find,f1,f2;

    GenData();

    fp1=fopen("sorted.txt","r");
    fp2=fopen("random.txt","r");
    Ts=CreateBST(fp1);
    Tr=CreateBST(fp2);
    fclose(fp1);
    fclose(fp2);

    while(1){
        manu();
        printf("Input：");
        scanf("%d",&ch);
        switch(ch) {
            case 1:printf("Input Data to [FIND] in SORTED：\t");
                scanf("%d",&key);
                cnt=0;
                find=Find(key,Ts);          ///find 作为标志，为1找到，为0未找到
                if(find==1)printf("Found!\n");
                else printf("NOT Found!\n");
                printf("CMP COUNT: %d\n",cnt);
                break;
            case 2:printf("Input Data to [FIND] in UNSORTED：\t");
                scanf("%d",&key);
                cnt=0;
                find=Find(key,Tr);
                if(find)printf("Found!\n");
                else  printf("NOT Found！\n");
                printf("CMP COUNT: %d\n",cnt);
                break;
            case 3:printf("Input Data to [DELETE]：\t");
                scanf("%d",&key);
                f1=Find(key,&Tr);
                if(f1) {
                    DeleteB(key,&Tr);
//                    Inorder(Tr,inorder);
                }
                else {printf("NOT Found in Tr！\n");}
                f2=Find(key,Ts);
                if(f2){
                    DeleteB(key,&Ts);
//                    Inorder(Ts,inorder);
                }
                else { printf("NOT Found in Ts！\n");}
                break;
            case 4:
                puts("");
                puts("[SORTED Order Create Tree]: ");
                Test(Ts);
                puts("");
                puts("[UNSORTED Order Create Tree]: ");
                Test(Tr);
                puts("");
                break;

            case 5:
                BST2InorderData(Ts);
                half_cnt=0;        ///每次调用折半查找之前，先将计数器置为0
                printf("Input Data to [FIND]：\t");
                scanf("%d",&key);
                if(Half_Search(key)) { printf("Found!\n"); }
                else{ printf("NOT Found!\n"); }
                printf("CMP COUNT：%d\n",half_cnt);
                break;

            case 6:
                puts("");
                BST2InorderData(Ts);
                printf("Test HALF_SEARCH：\n");
                Test_half();
                puts("");
                break;
            case 7:
                BST2InorderData(Ts);
                printf("Sorted List: ");
                for(int i=0;i<N;i++) {
                    printf("%d ",inorder[i]);
                }
                puts("");
                break;
            case 0:
                puts("Good Bye!!!");
                exit(0);
        }

    }

}
