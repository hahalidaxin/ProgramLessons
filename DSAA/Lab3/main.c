#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#define NAMESIZE 100
#define SIZE 100
#define FILENAME1 "ex3_Graph1.txt"
#define FILENAME2 "ex3_Graph2.txt"
typedef char NAME[NAMESIZE];

typedef struct graph
{
    int num;//顶点个数
    NAME name[SIZE];
    int  adj[SIZE][SIZE];
    int visited[SIZE];
    int dis[SIZE];
} Graph;
Graph G;

int menu();
void inigraph(char name[]);
void show(int num);
void DFS(int i);
void BFS(int i);
void SPFA(int i);

int menuMAIN()
{
    char op[2];
    printf("input your choice of Graph:[1][2]:\n");
    scanf("%s",op);
    if(strcmp(op,"1") && strcmp(op,"2")) {
        puts("error");
        return 0;
    }
    if(strcmp(op,"1")==0) {
        inigraph(FILENAME1);
    } else {
        inigraph(FILENAME2);
    }
    puts("iniGraph Done!");
    while(1) {
        puts("input your opration choice:");
        puts("[1] dfs search\t[2] bfs search [3]Shortest Path [4]exit");
        int op2;
        scanf("%d",&op2);
        memset(G.visited,0,sizeof(int)*G.num);
        switch (op2) {
            case 1:
                puts("DFS search result:");
                DFS(0);
                puts("");
                break;
            case 2:
                puts("BFS search result:");
                BFS(0);
                puts("");
                break;
            case 3:
                puts("SPFA result:");
                SPFA(0);
                puts("");
                break;
            case 4:
                return 0;
            default:
                puts("input error! Again =-=");
        }
    }
    return 0;
}

int main()
{
    while(1) {
        menuMAIN();
    }
}

void inigraph(char name[])
{
    FILE *fp = NULL;
    fp = fopen(name, "r");
    int num,i=0,j=0;
    fscanf(fp,"nodeNum:%d\n",&num);
    G.num=num;
    char input[100];
    fgets(input,25,fp);
    fgetc(fp);
    for (i=0; i<num; i++)
    {
        fscanf(fp,"%s",G.name[i]);
    }
    fgetc(fp);
    for (i=0; i<num; i++)
    {
        fscanf(fp,"%s",input);
        int x;
        for(x=0;x<G.num;x++)
            if(strcmp(G.name[x],input)==0)
                break;
        for (j=0; j<num; j++)
        {
            fscanf(fp,"%d",&G.adj[x][j]);
        }
    }
    fgetc(fp);
    for (i=0; i<num; i++)
    {
        G.visited[i]=0;
    }
    G.visited[i]=999;
    show (num);
}


void show(int num)
{
    int i=0,j=0;
    printf("LinkMattrix is :\n");
    for (i=0; i<num; i++)
    {
        for (j=0; j<num; j++)
        {
            printf("%d ",G.adj[i][j]);
        }
        printf("\n");
    }
}

void DFS(int i)
{
    int j;
    G.visited[i] = 1;
    printf("%s ",G.name[i]);
    for (j=0; j<G.num; ++j)
    {
        if (G.adj[i][j]>0&&!G.visited[j])
            DFS(j);
    }
}
void BFS(int i)
{
    memset(G.visited,0, sizeof(int)*G.num);
    int tmp;
    int queue[10000];
    G.visited[0]=1;
    int back = 0;
    int front = 0;
    queue[back]=i;
    back++;
    while(front!=back)
    {
        printf("%s ",G.name[queue[front]]);
        tmp=queue[front++];
        for(int j=0; j<G.num; j++)
        {
            if(G.adj[tmp][j]>0&&G.visited[j]==0)
            {
                G.visited[j]=1;
                queue[back]=j;
                back++;
            }
        }
    }
}

void SPFA(int S) {
    int queue[100000];
    int father[10000];
    int back = 0,front = 0;
    memset(G.visited,0, sizeof(int)*G.num);
    memset(father,0,sizeof(int)*G.num);
    queue[back++]=S;
    G.visited[S] = 1;
    G.dis[S] = 0;
    father[S] = -1;
    for(int i=1;i<G.num;i++)  G.dis[i] = 1e9;
    while(front!=back)  {
        int u = queue[front++];
        G.visited[u] = 0;
        for(int j=0; j<G.num; j++)  {
            if(G.adj[u][j]!=-1 && G.dis[u]+G.adj[u][j]<G.dis[j]) {
                G.dis[j] = G.dis[u]+G.adj[u][j];
                father[j] = u;
                if(!G.visited[j]) {
                    G.visited[j] = 1;
                    queue[back++] = j;
                }
            }
        }
    }
    for(int i=1;i<G.num;i++) {
        int stack[1000],cnt=0;
        printf("%s->%s path : ",G.name[S],G.name[i]);
        int x = i;
        while(father[x]!=-1) {
            stack[cnt++] = x;
            x = father[x];
        }
        stack[cnt++]=S;
        if(G.dis[x]==1e9) {
            puts("none path");
        } else {
            for(int j=cnt-1;j>0;j--){
                printf("%s,",G.name[stack[j]]);
            }
            printf("%s \t\t value = %d\n",G.name[stack[0]],G.dis[i]);
        }
    }
}
