//这个版本的文件标识符超了，而且说是fork了实际逻辑上是单进程的
// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user.h"

// /*
//     用currentPid记录当前的除数，每次加一，num记录仍待检验数字的数量
//     主程序先用while循环，验证数字并且创建管道（管道创建在一个二维数组中？）
//     while(num <= 0 并且 未到最后检验除数35){
//         fork()
//     }
//     每个进程负责输出它认为是质数的数字
// */
// int main()
// {

//   int pidNum = 2;
//   int primeNum = 0;
//   int p[34][2];

//   //创建管道
//   for (int i = 0; i < 34; i++)
//   {
//     pipe(p[i]);
//     int num = i+2;
//     write(p[i][1], &num, sizeof(num));
//   }

//   //判断质数
//   for (int i = 0; i < 34; i++)
//   {
//     int num;
//     read(p[i][0], &num, sizeof(num));
//     /*
//       如果数字小于等于pidNum则直接打印输出
//       如果数字大于pidNum且能够被整除则关闭管道
//       如果数字大于pidNum但不能够被整除则将该数字再次写入管道
//     */
//     if (num <= pidNum)
//     {
//       printf("prime number %d\n", num);
//     }
//     else if (num % pidNum == 0)
//     {
//       close(p[i][0]);
//       close(p[i][1]);
//     }
//     else
//     {
//       write(p[i][1], &num, sizeof(num));
//       primeNum++;
//     }
//   }

//   while (primeNum > 0 && pidNum <= 35)
//   {
//     if (fork() == 0)
//     { //子进程
//       pidNum++;
//       primeNum = 0; //重置质数数量的计数
//       for (int i = 0; i < 34; i++)
//       {
//         int num;
//         read(p[i][0], &num, sizeof(num));
//         if (num == 0) //表示管道关闭
//         {
//           close(p[i][0]);
//           close(p[i][1]);
//         }
//         else //表示读出了数据
//         {
//           if (num <= pidNum)
//           {
//             printf("prime %d\n", num);
//           }
//           else if (num % pidNum == 0)
//           {
//             close(p[i][0]);
//             close(p[i][1]);
//           }
//           else
//           {
//             write(p[i][1], &num, sizeof(num));
//             primeNum++;
//           }
//         }
//       }
//     }
//     else
//     { //父进程
//       for (int i = 0; i < 34; i++)
//       {
//         close(p[i][0]);
//         close(p[i][1]);
//       }
//       break;
//     }
//   }
//   wait(0);
//   exit(0);
// }

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

//要注意文件标识符有上限数量，所以可以将标准输入和输出的标识符作为管道标识符
//第一个父程序将2-35写进管道，其余子程序看情况来写
//第一个父程序要使用wait()函数等待其它子程序结束

void primes()
{
  int num, othNum;
  int p[2];
  if (read(0, &num, sizeof(int)) <= 0 || (num == 0) )
  {
    close(0);
    exit(0);
  }
  printf("prime %d\n", num); //打印第一个
  pipe(p);
  if (fork() == 0)
  { //子程序
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    primes();
  }
  else
  { //原程序
    close(1);
    dup(p[1]);
    close(p[0]);
    close(p[1]);
    while(read(0,&othNum,sizeof(int))>0 && (othNum != 0)){
      if (othNum % num != 0)
      {
        write(1, &othNum, sizeof(int));
      }
    }
    close(1);
    wait(0);
    exit(0);
  }
}

int main()
{
  int p[2];
  pipe(p);
  if (fork() == 0)
  {
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);
    primes();
  }
  else
  { //祖父程序
    close(1);
    dup(p[1]);
    close(p[0]);
    close(p[1]);
    for (int i = 2; i <= 35; i++)
    {
      write(1, &i, sizeof(int));
    }
    close(1);
    wait(0);  
  }
  exit(0);
}