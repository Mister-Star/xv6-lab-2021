//
// Created by 周慰星 on 2022/5/15.
//
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/sysinfo"

int main(int argc, char* argv[]) {
    if(argc != 1) {
        fprintf(2, "sysinfo need no param\n", argv[0]);
        exit(1);
    }
    struct sysinfo info;
    sysinfo(&info);
    printf("free sapce %d, used process num:%d\n", info.free_mem, info.free_proc);
    exit(0);
}