#include<stdio.h>
#include<string.h>


typedef unsigned char *byte_pointer;

struct staff{
	char* job;
	int age;
	float height;

};

union myunion{
    int x;
    char y;
};

enum DAY
{
      MON=1, TUE, WED, THU, FRI, SAT, SUN
};

void show_bytes(byte_pointer start,size_t len) {
	size_t i;
	for(i=0;i<len;i++) {
		printf(" %.2x",start[i]);
	}
	printf("\n");
}

int main()
{
    short varshort=12;
	int varint=1100;
	long varlong=123454;
	float varfloat=127.0123f;
	double vardouble=1223.2222;
	char varchar='a';
	int* varpointer = &varint;
	int vararray[4] = {1,3,4,5};
	enum DAY varenum=TUE;
	struct staff varstruct;
	varstruct.job = "student"; varstruct.age=12; varstruct.height = 170.7f;
	union myunion varunion;
	varunion.x = 133; varunion.y = 'z';

    printf("Name\t\tVal\t\tLoc\t\tHex\t\t\n");
	printf("%s\t\t%d\t\t%p\t","short",varshort,&varshort); show_bytes((byte_pointer)&varshort,sizeof(short));
	printf("%s\t\t%d\t\t%p\t","Int",varint,&varint); show_bytes((byte_pointer)&varint,sizeof(int));
	printf("%s\t\t%d\t\t%p\t","Long",varlong,&varlong); show_bytes((byte_pointer)&varlong,sizeof(long));
	printf("%s\t\t%f\t\t%p\t","Float",varfloat,&varfloat); show_bytes((byte_pointer)&varfloat,sizeof(float));
	printf("%s\t\t%lf\t\t%p\t","Double",vardouble,&vardouble); show_bytes((byte_pointer)&vardouble,sizeof(double));
	printf("%s\t\t%c\t\t%p\t","Char",varchar,&varchar); show_bytes((byte_pointer)&varchar,sizeof(char));
	printf("%s\t\t%p\t\t%p\t","Pointer",varpointer,&varpointer); show_bytes((byte_pointer)&varpointer,sizeof(void*));
	printf("%s\t\t%d\t\t%p\t","Enum",varenum,&varenum); show_bytes((byte_pointer)&varenum,sizeof(enum DAY));
	printf("\n\nStruct:\n");
	printf("[Value]\njob:%s   age:%d   height:%f\n",varstruct.job,varstruct.age,varstruct.height);
	printf("[Location]\njob:%p   age:%p   height:%p\n",&varstruct.job,&varstruct.age,&varstruct.height);
	printf("[HEXTYPE]\t\n");
    printf("Job: "); show_bytes((byte_pointer)&varstruct.job,sizeof(char)*strlen(varstruct.job));
    printf("Age: "); show_bytes((byte_pointer)&varstruct.age,sizeof(int));
    printf("Height: "); show_bytes((byte_pointer)&varstruct.height,sizeof(float));
    printf("\n");
    printf("\n\nArray:\n");
    printf("[Value]\n");
    int i;
    for(i=0;i<4;i++) {
        printf("index: %d val: %d\n",i,vararray[i]);
    }
    printf("\n");
    printf("[Location]\n");
    for(i=0;i<4;i++) {
        printf("index: %d Loc: %d\n ",i,&vararray[i]);
    }
    printf("[HEXType]\n");
    for(i=0;i<4;i++) {
        printf("index: %d Hex: ",i);
        show_bytes((byte_pointer)vararray+i,sizeof(int));
    }
    printf("\n\nUnion:\n");
	printf("[Value]\nx:%d   y:%c\n",varunion.x,varunion.y);
	printf("[Location]\nx:%p   y:%p  \n",&varunion.x,&varunion.y);
	printf("[HEXTYPE]\t\n");
    printf("x: "); show_bytes((byte_pointer)&varunion.x,sizeof(int));
    printf("y: "); show_bytes((byte_pointer)&varunion.y,sizeof(char));
    printf("\n");

    printf("Location of void [printf]: %p\n",printf);
    printf("Location of int [main]: %p\n\n",main);
}
