//
// Created by 周慰星 on 2022/5/8.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find_helper(char const *path, char const *filename)
{

    char buf[512], *p;
    int fd;//文件描述符
    struct dirent de;//
    struct stat st; //文件信息

    if((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        exit(1);
    }

    switch(st.type) {
        case T_FILE:
            printf("Usage: find dir file\n");
            exit(1);
        case T_DIR:
            if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
                printf("path is too long\n");
                break;
            }
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';
            while(read(fd, &de, sizeof de) == sizeof de) {
                if(de.inum == 0) continue;
                memmove(p, de.name, DIRSIZE);//添加路径到buf中
                p[DIRSIZE] = 0;//添加字符串结束标志
                if(stat(buf, &st) < 0) {
                    fprintf(2, "find: cannot stat %s\n", buf);
                    continue;
                }
                if(st.type == T.DIR && strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0) {
                    find_helper(buf, filename)
                }
                else if(strcmp(filename, de.name) == 0) {
                    printf("%s\n", buf);
                }
            }
            break;
    }
    close(fd);
}
int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find dir file\n");
        exit(1);
    }

    char const *path = argv[1];
    char const *target = argv[2];
    find_helper(path, target);
    exit(0);
}