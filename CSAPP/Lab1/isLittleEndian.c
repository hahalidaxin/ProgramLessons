
#include<stdio.h>



int isLittleEndian()
{
	union temp
	{
		short int a;
		char b;
	}temp;
	temp.a = 0x1234;
	printf("%d",temp.a);
	if( temp.b == 0x12 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main()
{
    int ans = isLittleEndian();
    if(ans==0) {
        printf("ur PC is Little Endian");
    } else if (ans==1) {
        printf("ur PC is Big Endian");
    }
}
