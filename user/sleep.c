//
// Created by 周慰星 on 2022/5/8.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(2, "args not match\n");
    }
    else {
        int time = atoi(argv[2]);
        sleep(time);
    }
    exit(0);
}