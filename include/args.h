/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DCLI_ARGS_H
#define DCLI_ARGS_H

#define ARGS_MAX_POS 128

struct parsed_args {
    char *host;                        /* --host / -H 指定的值，可能为 NULL */
    const char *pos[ARGS_MAX_POS];     /* 剩余位置参数 */
    int pos_count;
};

/*
 * 解析命令行参数。
 * argc/argv 是 main 的参数（argv[0] 是程序名）。
 * 出错时直接 exit(2)。
 */
void args_parse(int argc, char **argv, struct parsed_args *out);

#endif /* DCLI_ARGS_H */
