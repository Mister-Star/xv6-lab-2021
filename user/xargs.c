//
// Created by 周慰星 on 2022/5/8.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define MAXSZ 512

enum state{
    S_WAIT, //等待参数输入 当前为空格
    S_ARG, //参数输入
    S_ARG_END, //参数输入结束
    S_AGE_LINE_END, //参数后接换行
    S_LINE_END, //空格后接换行
    S_END //结束
};

enum char_type {
    C_SPACE,
    C_CHAR,
    C_LINE_END
};

enum char_type get_char_type(char c) {
    switch (c) {
        case ' ' :
            return C_SPACE;
        case '\n' :
            return C_LINE_END;
        default :
            return C_CHAR;
    }
}

enum state transform_state(enum state cur, enum char_type ct) {
    switch (cur) {
        case S_WAIT :
            if(ct == C_CHAR) return S_ARG;
            if(ct == C_SPACE) return S_WAIT;
            if(ct == C_LINE_END) return S_LINE_END;
            break;
        case S_ARG :
            if(ct == C_CHAR) return S_ARG;
            if(ct == C_SPACE) return S_ARG_END;
            if(ct == C_LINE_END) return S_AGE_LINE_END;
            break;
        case S_ARG_END:
        case S_AGE_LINE_END:
        case S_LINE_END:
            if(ct == C_CHAR) return S_ARG;
            if(ct == C_SPACE) return S_WAIT;
            if(ct == C_LINE_END) return S_LINE_END;
            break;
        default:
            break;
    }
    return S_END;
}

int main(int argc, char* args[]) {
    if(argc - 1 > MAXSZ) {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }

    char lines[MAXSZ];
    char *p = lines;
    char *x_args[MAXSZ] = {0};

    for(int i = 1; i < argc; i ++) {
        x_args[i - 1] = agrs[i];
    }
    int arg_begin = 0, arg_end = 0, arg_cnt = argc - 1;
    enum state st = S_WAIT;

    while(st != S_END) {
        if(read(0, p, sizeof char) != sizeof char)
            st = S_END;
        else
            st = transform_state(st, get_char_type( *p ));

        if( ++ arg_end >= MAXSZ) {
            fprintf(2, "xargs: arguments too long.\n");
            exit(1);
        }

        switch (st) {
            case S_WAIT :
                ++ arg_begin;
                break;
            case S_ARG_END :
                x_args[arg_cnt++] = &lines[arg_begin];
                arg_begin = arg_end;
                *p = '\0';
                break;
            case S_AGE_LINE_END :
                x_args[arg_cnt++] = &lines[arg_begin];
            case S_LINE_END :
                arg_begin = arg_end;
                *p = '\0';
                if(fork() == 0) {
                    exec(argv[1], x_args);
                }
                arg_cnt = argc - 1;
                for(int i = arg_cnt; i < MAXSZ; i ++) {
                    x_args[i] = 0;
                }
                wait(0);
                break;
            default:
                break;
        }
        p ++;
    }
    return 0;
}

