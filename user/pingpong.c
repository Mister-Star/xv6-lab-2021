//
// Created by 周慰星 on 2022/5/8.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int pid;
    int p[2];
    pipe(p);
    if(fork() == 0) {
        pid = getpid();
        char buf[3];
        if(read(p[0], buf, 1) != 1) {
            fprintf(2, "child read failed\n");
            exit(1);
        }
        close(p[0]);
        printf("%s\n", buf);
        printf("%d: received ping\n", pid);
        if(write(p[1], buf, 1) != 1) {
            fprintf(2, "child write failed\n");
            exit(1);
        }
        close(p[1]);
        exit(0);
    }
    else {
        pid = getpid();
        char info[3] = "zw", buf1[3] = "1";
        if(write(p[1], info, 1) != 1) {
            fprintf(2, "parent write failed\n");
            exit(1);
        }
        close(p[1]);
        wait(0);
        if(read(p[0], buf1, 1) != 1) {
            fprintf(2, "parent read failed\n");
            exit(1);
        }
        printf("%s\n", buf1);
        printf("%d: received pong\n", pid);
        close(p[0]);
        exit(0);
    }
//    int pid;
//    int p[2];
//    pipe(p);
//
//    if (fork() == 0) // child (receive -> send)
//    {
//        pid = getpid();
//        char buf[2];
//        if (read(p[0], buf, 1) != 1)
//        {
//            fprintf(2, "failed to read in child\n");
//            exit(1);
//        }
//        close(p[0]);
//        printf("%d: received ping\n", pid);
//        if(write(p[1], buf, 1) != 1)
//        {
//            fprintf(2, "failed to write in child\n");
//            exit(1);
//        }
//        close(p[1]);
//        exit(0);
//    }else{			// parent (send -> receive)
//        pid = getpid();
//        char info[2] = "a";
//        char buf[2];
//        buf[1] = 0;
//        if (write(p[1], info, 1) != 1)
//        {
//            fprintf(2, "failed to write in parent\n");
//            exit(1);
//        }
//        // wait for child to receive ping
//        close(p[1]);
//        wait(0);
//        if(read(p[0], buf, 1) != 1){
//            fprintf(2, "failed to read in parent\n");
//            exit(1);
//        }
//        printf("%d: received pong\n", pid);
//        close(p[0]);
//        exit(0);
//    }
}