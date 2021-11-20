#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user.h"

//读取输入
//调用exec函数
int main(int argc, char *argv[])
{
    int index,len;
    char buf[1024];
    char *exec_argv[MAXARG]; //MAXARG表示max exec arguments
    for (int i = 0; i < argc; i++)
    {
        exec_argv[i - 1] = argv[i];
    }

    while (1)
    {
        index = -1;
        do {
            index++;
            len = read(0, &buf[index], sizeof(char));
        } while (len > 0 && buf[index] != '\n');
        
        if(len == 0) break;//表示输入已经到了末尾，退出循环
        buf[index] = '\0';
        exec_argv[argc-1] = buf;

        if (fork() == 0)
        { //子进程
            exec(exec_argv[0], exec_argv);
            exit(0);
        }
        else
        { //父进程
            wait(0);
        }
    }
    exit(0);
}