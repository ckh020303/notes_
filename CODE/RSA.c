#include <stdio.h>
int candp(int a,int b,int c)
{ 
	int r=1;
	b=b+1;
	while(b!=1)
	{
	    r=r*a;
	    r=r%c;
	    b--;
	}
	
	return r;
}
int main()
{
	long long int p,q,e,d,n,t,c,r,a=0;
	char m[16]="a465d75cdf9240b9";
	char c[32]="";
	char s;
    p=2350951035864083;
    q=7560136662999737;
	n=p*q;
	t=(p-1)*(q-1);
    printf("n=%d\n",n);
	printf("t=%d\n",t);
	printf("请输入 e: ");
	scanf("%d",&e);
	if(e<1||e>t)
	{
	     printf("输入的e不合规，请重新输入 ");
	     scanf("%d",&e);
	}
	d=1;
	while(((e*d)%t)!=1)   d++;
	printf("计算d的结果为 %d\n",d);
	while(1)
	{
		printf("加密请输入 1\n");
		printf("解密请输入 2\n");
		printf("退出请输入 3\n");
		scanf("%d",&r);
		switch(r)
		{
			case 1: printf("使用私钥生成签名\n");
					printf("MD5字符串：%s\n",m);
					c=candp(m,d,n);
					printf("密文为 %d\n",c);break;
			case 2: printf("使用公钥验证签名\n");
					m=candp(c,e,n);
					printf("明文为 %d\n",m);break;
			case 3: a=1;break;
		}
		getchar();
		if(a==1){break;}
		a=0;
	}
	return 0;
}
