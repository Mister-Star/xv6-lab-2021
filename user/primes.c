//
// Created by 周慰星 on 2022/5/8.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void new_peoc(int p[2]) {
    int prime, n;
    close(p[1]);
    if(read(p[0], &prime, 4) != 4) {
        fprintf(2, "process failed to read from the pipe\n");
        exit(1);
    }
    printf("prime %d\n", prime);
    if(read(p[0], &n, 4)) {
        int newp[2];
        pipe(newp);
        if(fork() == 0) {
            new_peoc(newp);
        }
        else {
            close(newp[0]);
            if(n % prime != 0) write(newp[1], &n, 4);
            while(read(p[0], &n, 4)) {
                if(n % prime != 0) write(newp[1], &n, 4);
            }
            close(p[0]);
            close(newp[1]);
            wait(0);

        }
    }
    exit(0);
}


int main(int argc, char *argv[]) {
    int p[2];
    pipe(p);
    if(fork() == 0) {
        new_peoc(p);
    }
    else {
        close(0);
        for(int i = 2; i <= 35; i ++) {
            if(write(p[1], &i, 4) != 4) {
                fprintf(2, "first process failed to write %d into the pipe\n", i);
                exit(1);
            }
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
    return 0;
}