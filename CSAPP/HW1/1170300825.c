//《深入理解计算机系统》第二章作业


#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<math.h>

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
/*
    写出相应函数
    2.57 的答案如下：
*/
typedef unsigned char *byte_pointer;
void show_bytes(byte_pointer start,size_t len) {
	size_t i;
	for(i=0;i<len;i++) {
		printf(" %.2x",start[i]);
	}
	printf("\n");
}
void show_short(short varshort) {
    show_bytes((byte_pointer)&varshort,sizeof(short));
}
void show_long(long varlong) {
    show_bytes((byte_pointer)&varlong,sizeof(long));
}
void show_double(double vardouble) {
    show_bytes((byte_pointer)&vardouble,sizeof(double));
}

/*
    条件表达式
    2.61 的答案如下：
    	(!((x&2147483647)^2147483647) || !x || !((x&15)^15) ||  !(x&2013265920))
*/

int testexpression(int x) {
    if (!((x&2147483647)^2147483647) || !x || !((x&15)^15) ||  !(x&2013265920))
        return 1;
    else
        return 0;
}

/*
    判断二进制格式中1的数目是否为奇数
    2.65 的答案如下：
*/
int odd_ones(unsigned x) {
    int cnt = 0;
    while(x) {
        if (x&1) {
            cnt += 1;
        }
        x>>=1;
    }
    return cnt&1;
}

/*
    实现循环左移的位运算
    2.69 的答案如下：
*/
unsigned rotate_left(unsigned x,int n) {
    unsigned mask = 1<<31;
    while(n) {
        printf("%u\n",x&mask);
        x =  (x&mask)!=0 | (x<<1) ;
        n -= 1;
    }
    return x;
}
/*
    饱和加运算
    2.73 的答案如下：
*/
int saturating_add(int x,int y) {
    unsigned w = sizeof(int) << 8;
    int a = (x ^ y) >> (w-1);
    int b = ((x+y) ^ x) >> (w-1);
    int c = x >> (w-1);
    return (a&(x+y))+(~a&(b&((~c&INT_MAX)+(c&INT_MIN) )));
}
/*
    2.77 的答案如下：
        K=17  (x<<4)+x
        K=-7   x-(x<<3)
        K =60  (x<<6)-(x<<2)
        K=-112 (x<<4)-(x<<7)
*/

/*
    2.81 的答案如下：
    A: ~((1<<k)-1)
    B:((1<<k)-1)<<j
*/


/*
    2.85 的答案如下： 
    （声明：其中0b..为二进制表示，其中位表示的符号，阶码和尾码之间用,隔开） 
    A: 7
        E = 2
        M = 0b1.11
        f = 0b0.11 
        V = 7.0
        位表示的描述：0 , 100...01 , 1100....
    B:能够被准确描述的最大奇整数
        E = n
        M = 0b1.111... （一共n个1） 
        f = 0b0.111...（一共n个1） 
        V = 0b111...（一共n+1个1） 
        位表示的描述:0， ((1<<k-1)-1)+n , 111...(一共n个1)
    C:最小的规格化数的倒数（令bias=(1<<k-1)-1) 
        E =  bias-1
        M = 0b1.000...
        f =	0b0.000...
        V =	2^(bias-1)
        位表示的描述 0, 11...101 ,000...
*/


/*
    2.89 的答案如下：
    A:恒为真 数学原理：首先int->double的转换中不会有精度损失，因此x和dx不会因为精度损失而不同，所以两者在转换成为float之后恒相等
    B：不恒为真 反例：x=-2147483648 y=1 
    C: 恒为真 数学原理：考虑极限情况，当int为32位时，两个int相加为33位。此时double可以表达不会产生精度误差。(dx=1e30,dy=1e30,dz=1e-30)
	D: 不恒为真 dx和dy是倒数关系，而且满足dz*dy=INT_MAX
	E：不恒为真，dy=0.0,dx=1.0
*/

/*
    2.93 的答案如下：
*/
typedef unsigned float_bits; 
float_bits float_absval(float_bits f) {
  unsigned sign = f >> 31;
  unsigned exp = f >> 23 & 0xFF;
  unsigned frac = f & 0x7FFFFF;
	
  //判断是不是NaN 
  if ((exp == 0xFF) && (frac != 0)) {
//    printf("yes\n");
	return f;	
  }
  else //省去符号位 返回绝对值  //其中0<<31用来保证补全32位 
  	return (exp<<23) | frac | 0<<31;
}

/* 题目2.93的测试函数 */ 
float u2f(unsigned x){
	//位填充 直接改变指针完成转换 
    return *((float*)&x);
}
float exceptnan(float_bits x) {
	//转换 同时排除nan的情况 
	float fn =  u2f(x);
	if(isnan(fn)) return 0.0;
	else return fn;
}
void test4_float_absval() { 
//	unsigned f = 0xFFFFFFFF;	//NaN
//	unsigned f2 = 0xF000FFFF;
//	printf("%u %u",f2,float_absval(f2));
	unsigned i;
	for (i=3472883657;i<0xFFFFFFFF;i++) {
		float fa = exceptnan(float_absval(i));
		float fb = fabs(exceptnan(i));
		printf("%f %f\n",fa,fb);
//		float f = u2f((unsigned)i);
//		printf("%u %f\n",i,f);
	}
}


/*
    2.97 的答案如下：
*/
//获取位长 最高位为1位为止 返回值比1的所在大1位 
int bitsLen(int i) {
  //判断100000... 
  if ((i & INT_MIN) != 0) {
    return 32;
  }
  unsigned x = (unsigned)i;
  int len = 0;
  //try 
  while (x >= (1<<len)) len++;
  return len;
}
//获得前高l位都是1的数 
unsigned mask(int l) {
//  printf("xx:  %d %u\n",l,-1<<(32-l));
  return (unsigned) -1>>(32-l);
}
unsigned f2u(float x)
{
	return *(unsigned *)&x;
}
float_bits float_i2f(int i) {
  unsigned sig, exp, frac, round_part;
  unsigned bias = 0x7F;

  if (i == 0) {
  	//判断是0的情况 
    return 0<<31; 
  }
  if (i == INT_MIN) {
  	//判断是int最小值的情况 
    return 1<<31|(bias+31)<<23|0<<31;
  }
  //去掉符号位 取得值的绝对值 
  if (i < 0) {
    sig = 1;
    i = -i;
  } else {
  	sig = 0;
  }
  //位长 
  unsigned fbits = bitsLen(i)-1;
  exp = bias + fbits;				//exp 
  unsigned rest = i&mask(fbits);
  unsigned exp_sig;
  if (fbits<=23) {
  	//不用处理进位 
    frac=rest<<(23-fbits);
    exp_sig=exp<<23|frac;
  } else {
  	//处理进位的情况 核心部分 
    int jinwei_mid = 1<<(fbits - 23 - 1);
    round_part=rest&mask(fbits-23);
    frac=rest>>(fbits-23);
    exp_sig=exp<<23|frac; 
    if (round_part>jinwei_mid) {
      //大于一半需要进位 
      exp_sig+=1;
    } else if(round_part==jinwei_mid) {
      //如果正好一半 则向偶数进位 
      if ((frac&1)==1) exp_sig++;
    }
  }
  return sig<<31|exp_sig;
}

void test4_float_i2f() {
	int i;	
	for (i=INT_MIN;i<INT_MAX;i++) {
		float f1 = (float)(i);
		float f2 = u2f(float_i2f((unsigned)i));
		printf("%f %f\n",f1,f2);
	}
}

int main(){	
//	test4_float_i2f();
	test4_float_absval();
	return 0;
}
