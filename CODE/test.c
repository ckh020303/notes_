#include <stdio.h>
#include <stdlib.h>
#define N 100
// int aaa(int n){
//     int num=1;
//     if (n==10)
//     {
//         printf("第%d天：%d\n",n,num);
//         return 1;
//     }
//     else
//     {
        
//         num = (aaa(n+1) + 1)*2;
//         printf("第%d天：%d\n",n,num);
//         return num;
//     }

// } 
//a(1) = (a(2) + 1)* 2  a(2) = (a(3)+1)*2 a(3)=1

// int bbb(int n){
//     int num=10;
//     if(n==1){
//         printf("第%d个人：%d\n",n,num);
//         return 10;
//     }
//     else{
//         num = bbb(n-1)+2;
//         printf("第%d个人：%d\n",n,num);
//         return num;
//     }
// }
//b(5)=b(4)+2
int main(){
    // printf("%d",bbb(5));
    // printf("%d",aaa(1));
    FILE* fp;
    fp = fopen("X:\\CODE\\codeFile\\helloworld.txt", "r");
    if (fp != NULL)
    {
        //feof(file stream )文件指针到达文件末尾
        while (!feof(fp)) //读文件
            printf("%c", fgetc(fp));
    }
    else
        printf("fail to open! \n");
    fclose(fp);
    return 0;
}

