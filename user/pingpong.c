// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user.h"

// int main(int argc, char *argv[]){
//     char c ='a';
//     int readNum;
//     int p1[2]; //父进程写，子进程读
//     int p2[2]; //子进程写，父进程读
//     pipe(p1);
//     pipe(p2);
//     if(fork() == 0){
//         close(p1[1]);
//         close(p2[0]);
//         readNum = read(p1[0],&c,1);
//         if(readNum != 1){
//             fprintf(2, "child read error\n");
//             exit(1);
//         }
//         printf("%d: received ping\n",getpid());
//         write(p2[1],&c,1);
//         close(p1[0]);
//         close(p2[1]);
//         exit(0);
//     }else{
//         close(p1[0]);
//         close(p2[1]);
//         write(p1[1],&c,1);
//         readNum = read(p2[0],&c,1);
//         if(readNum != 1){
//             fprintf(2, "parent read error\n");
//             exit(1);
//         }
//         printf("%d: received pong\n",getpid());
//         close(p1[1]);
//         close(p2[0]);
//         wait(0);
//         exit(0);
//     }
// }
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

//一定要注意输出的内容一致性！！
int main(){
    char buf[100]; //存储读取的数据
    int readNum; //记录读取的数据量
    int ptc[2]; //父进程写，子进程读
    int ctp[2]; //子进程写，父进程读
    pipe(ptc);
    pipe(ctp);
    if(fork() == 0){
        close(ptc[1]);
        close(ctp[0]);
        readNum = read(ptc[0],buf,100);
        if(readNum > 0){
            printf("%d: received ping\n",getpid());
        }
        write(ctp[1],"hello,father",strlen("hello,father"));
        close(ptc[0]);
        close(ctp[1]);
        exit(0);
    }else{
        close(ptc[0]);
        close(ctp[1]);
        write(ptc[1],"hello,child",strlen("hello,child")); //1为写端，0为读端
        readNum = read(ctp[0],buf,100);
        if(readNum > 0){
            printf("%d: received pong\n",getpid());
        }
        close(ptc[1]);
        close(ctp[0]);
        wait(0);
        exit(0);     
    }
}