#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void Swap(int &x,int &y)
{
    int temp;
    temp=x;
    x=y;
    y=temp;
}
void  BubbleSort(int n,int *num)//冒泡
{
    int i,j;
    for(i=0; i<=n-1; i++)
    {
        for(j=n-1; j>=i+1; j--)
        {
            if(num[j]<num[j-1])
            {
                Swap(num[j],num[j-1]);
            }
        }
    }
}

int FindPivot(int *num,int i,int j)//基准元素的选取
{
    int firstkey=num[i];
    int k;
    for(k=i+1; k<=j; k++)
    {
        if(num[k]>firstkey)
            return k;
        else if(num[k]<firstkey)
            return i;
    }
    return -1;
}
int Partition(int *num,int i,int j,int pivot)//无序区划分
{
    int l,r;

    do
    {
        for(l=i; num[l]<pivot; l++);
        for(r=j; num[r]>=pivot; r--);
        if(l<r)
            Swap(num[l],num[r]);
    }
    while(l<=r);
    return l;
}
void  QuickSort(int *num,int i,int j)//快排
{
    int pivot;
    int k;
    int pivotindex;
    pivotindex=FindPivot(num,i,j);
    if(pivotindex!=-1)
    {
        pivot=num[pivotindex];
        k=Partition(num,i,j,pivot);
        QuickSort(num,i,k-1);
        QuickSort(num,k,j);
    }
}

int *num; int *test1;
int *num1; int *test4;
int *num2; int *test7;

void genData(int n) {
    num = (int *) malloc(sizeof(int) * n);
    test1 = (int *) malloc(sizeof(int) * n);
    num1 = (int *) malloc(sizeof(int) * n);
    test4 = (int *) malloc(sizeof(int) * n);
    num2 = (int *) malloc(sizeof(int) * n);
    test7 = (int *) malloc(sizeof(int) * n);
    int i;

    srand((unsigned) time(NULL));
    for (i = 0; i < n; i++) {
        num1[i] = i;
        num[i] = i;
        num2[i] = n-i-1;
        test4[i] = i;
        test7[i] = n-i-1;
        test1[i] = i;
    }
    for (i = 0; i < n; i++) {
        int x = rand() % n;
        Swap(num[i],num[x]);
        Swap(test1[i],test1[x]);
    }

}

void genFileOutPut() {
    FILE *QS_sorted = fopen("QS_sorted.txt","w");
    FILE *QS_inverted = fopen("QS_inverted.txt","w");
    FILE *QS_random = fopen("QS_random.txt","w");

    FILE *BS_sorted = fopen("BS_sorted.txt","w");
    FILE *BS_inverted = fopen("BS_inverted.txt","w");
    FILE *BS_random = fopen("BS_random.txt","w");

    puts("----- gen data start -----");

    puts("Data for Bubble_Sort in [0,10000]");
    DWORD start_time;
    DWORD end_time;

    for(int n=0;n<=10000;n+=100)
    {
        printf("Data %d Output:\n",n);

        genData(n);

        //sorted test4
        //inverted test7
        //random test1
        start_time = GetTickCount();
        BubbleSort(n, test4);
        end_time = GetTickCount();
        fprintf(BS_sorted,"%d\n",end_time-start_time);
        puts("BS_sorted done");

        start_time = GetTickCount();
        BubbleSort(n, test7);
        end_time = GetTickCount();
        fprintf(BS_inverted,"%d\n",end_time-start_time);
        puts("BS_inverted done");

        start_time = GetTickCount();
        BubbleSort(n, test1);
        end_time = GetTickCount();
        fprintf(BS_random,"%d\n",end_time-start_time);
        puts("BS_random done");

    }

    puts("Data for Quick_Sort in [0,1000000]");
    for(int n=0;n<=100000;n+=1000)
    {
        printf("Data %d Output:\n",n);

        genData(n);

        //sorted num1
        //inverted num2
        //random num
        start_time = GetTickCount();
        QuickSort(num1, 0, n - 1);
        end_time = GetTickCount();
        fprintf(QS_sorted,"%d\n",end_time-start_time);
        puts("QS_sorted done");

        start_time = GetTickCount();
        QuickSort(num2, 0, n - 1);
        end_time = GetTickCount();
        fprintf(QS_inverted,"%d\n",end_time-start_time);
        puts("QS_inverted done");

        start_time = GetTickCount();
        QuickSort(num, 0, n - 1);
        end_time = GetTickCount();
        fprintf(QS_random,"%d\n",end_time-start_time);
        puts("QS_random done");
    }


    fclose(BS_sorted);
    fclose(BS_inverted);
    fclose(BS_random);
    fclose(QS_sorted);
    fclose(QS_inverted);
    fclose(QS_random);

}

void menu()
{
    printf("1.Sorted Data\n");
    printf("2.Inverted Data\n");
    printf("3.Random Data\n");
    puts("4.Gen Picture Data");
    puts("5.Exit");
}


int main() {
    int n, i, choice;
    printf("Input the size of Array\n");
    scanf("%d", &n);

    genData(n);

    DWORD start_time1;
    DWORD start_time2;
    DWORD end_time1;
    DWORD end_time2;
    while (1) {
        start_time1 = 0;
        start_time2 = 0;
        end_time1 = 0;
        end_time2 = 0;
        menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                start_time1 = GetTickCount();
                BubbleSort(n, test4);
                end_time1 = GetTickCount();
                start_time2 = GetTickCount();
                QuickSort(num1, 0, n - 1);
                end_time2 = GetTickCount();
                printf("Bubble_Sort TIME: %d\n", (end_time1 - start_time1));
                printf("Quick_Sort TIME: %d\n", (end_time2 - start_time2));
                puts("");
                break;
            case 2:
                start_time1 = GetTickCount();
                BubbleSort(n, test7);
                end_time1 = GetTickCount();
                start_time2 = GetTickCount();
                QuickSort(num2, 0, n - 1);
                end_time2 = GetTickCount();
                printf("Bubble_Sort TIME: %d\n", (end_time1 - start_time1));
                printf("Quick_Sort TIME: %d\n", (end_time2 - start_time2));
                puts("");
                break;
            case 3:
                start_time1 = GetTickCount();
                BubbleSort(n, test1);
                end_time1 = GetTickCount();
                start_time2 = GetTickCount();
                QuickSort(num, 0, n - 1);
                end_time2 = GetTickCount();
                printf("Bubble_Sort TIME: %d\n", (end_time1 - start_time1));
                printf("Quick_Sort TIME: %d\n", (end_time2 - start_time2));
                puts("");
                break;
            case 4:
                genFileOutPut();
                break;
            case 5:
                free(num);
                free(num1);
                free(num2);
                free(test1);
                free(test4);
                free(test7);
                return 0;
            default:
                printf("Input Error\n");
        }
    }
}