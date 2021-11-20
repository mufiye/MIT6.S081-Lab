#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

//argv[1]是目录,argv[2]是文件名
/*遍历目录,
    1. 如果是文件就输出就输出这个结果
    2. 如果是目录就递归find函数
*/

void find(char* pathName, char* fileName){
    char buf[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if((fd = open(pathName,0)) < 0){
        fprintf(2, "find: cannot open %s\n", pathName);
        exit(1);
    }
    if(fstat(fd,&st) < 0){
        fprintf(2, "find: cannot stat %s\n", pathName);
        close(fd);
        exit(1);
    }
    
    if(st.type == T_DIR){
        strcpy(buf, pathName);
        p = buf + strlen(buf);
        *p++ = '/';
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
				continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if(stat(buf, &st) < 0){
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            switch (st.type)
            {
            case T_FILE:
                if(strcmp(de.name,fileName) == 0){
                    printf("%s\n",buf);
                } 
                //printf("%s\n",buf);
                break;
            case T_DIR:
                find(buf,fileName);
                break;
            }
        } 
    }
}

int main(int argc, char *argv[]){
    if(argc < 3){
        printf("wrong command arguments numbers\n");
        exit(1);
    }

    find(argv[1], argv[2]);
    exit(0);
}

