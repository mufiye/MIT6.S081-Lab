//为了测试gdb是否可用
#include "kernel/types.h"
#include "user.h"

int parse_int(const char* arg) {
    const char* p = arg;
    for ( ; *p ; p++ ) {
        if ( *p < '0' || *p > '9' ) {
            return -1;
        }
    }
    return atoi(arg);
}

int main(int argc,char** argv) {
    int time;
    if (argc != 2) {
        printf("you must input one argument only\n");
        exit(0);
    } 
    
    time = parse_int(argv[1]);
    if (time < 0) {
        printf("error argument : %s\n",argv[1]);
        exit(0);
    }
    sleep(time);
    exit(0);
}

// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user.h"

// int main(int argc, char *argv[]){
//     int x;
//     if(argc < 2){
//         fprintf(2, "Error message\n");
//         exit(1);
//     }
//     x = atoi(argv[1]);
//     sleep(x);
//     exit(0);
// }
