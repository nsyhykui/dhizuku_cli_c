/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "common.h"
#include "i18n.h"
#include "help.h"
#include "args.h"
#include "config.h"
#include "net.h"

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    i18n_init();

    struct parsed_args pa;
    args_parse(argc, argv, &pa);

    /* 无参数 → 全局帮助 */
    if (pa.pos_count == 0) {
        help_print_global();
        return 0;
    }

    /* dcli help / dcli --help */
    if (strcmp(pa.pos[0], "help") == 0 ||
        strcmp(pa.pos[0], "--help") == 0) {
        if (pa.pos_count < 2) help_print_global();
        else help_print_command(pa.pos[1]);
        return 0;
    }

    /* dcli <cmd> --help / dcli <cmd> -h */
    if (pa.pos_count >= 2 &&
        (strcmp(pa.pos[1], "--help") == 0 ||
         strcmp(pa.pos[1], "-h") == 0) &&
        help_is_known_command(pa.pos[0])) {
        help_print_command(pa.pos[0]);
        return 0;
    }

    /* 拼命令行 */
    char cmd_line[BUF_SIZE] = {0};
    for (int i = 0; i < pa.pos_count; i++) {
        if (i > 0) {
            strncat(cmd_line, " ",
                    sizeof(cmd_line) - strlen(cmd_line) - 1);
        }
        strncat(cmd_line, pa.pos[i],
                sizeof(cmd_line) - strlen(cmd_line) - 1);
    }

    /* 读密钥 */
    char *key = config_load_key();
    if (!key) {
        fprintf(stderr, "%s\n",
                tr("Error: no key found", "错误: 未找到密钥"));
        fprintf(stderr, "%s\n",
                tr("Copy the key from the App to ~/.dcli_key",
                   "请从 App 复制密钥，写入 ~/.dcli_key"));
        fprintf(stderr, "%s\n",
                tr("or set env DCLI_KEY", "或设置环境变量 DCLI_KEY"));
        return 1;
    }

    /* 读 host */
    char *host = config_load_host(pa.host);

    /* 发送 */
    char reply[BUF_SIZE];
    int rc = net_send_command(host, key, cmd_line, reply, sizeof(reply));

    free(key);
    free(host);

    if (rc < 0) {
        fprintf(stderr, "%s\n", reply);
        return 1;
    }

    /* 解析响应 */
    if (strcmp(reply, "Success") == 0) {
        printf("Success\n");
        return 0;
    }
    if (strncmp(reply, "Success ", 8) == 0) {
        printf("%s\n", reply + 8);
        return 0;
    }

    printf("%s\n", reply);
    return 1;
}
