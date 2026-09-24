/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "i18n.h"

static void usage_error(const char *a) {
    fprintf(stderr, "%s %s\n",
            tr("Error: missing argument after", "错误: 后缺参数"), a);
    exit(2);
}

static void unknown_opt_error(const char *a) {
    fprintf(stderr, "%s %s\n",
            tr("Error: unknown option:", "错误: 未知选项:"), a);
    exit(2);
}

void args_parse(int argc, char **argv, struct parsed_args *out) {
    out->host = NULL;
    out->pos_count = 0;

    int stop_parse = 0;

    for (int i = 1; i < argc; i++) {
        if (stop_parse) {
            if (out->pos_count >= ARGS_MAX_POS) break;
            out->pos[out->pos_count++] = argv[i];
            continue;
        }

        if (strcmp(argv[i], "--") == 0) {
            stop_parse = 1;
            continue;
        }

        if (strcmp(argv[i], "--host") == 0 || strcmp(argv[i], "-H") == 0) {
            if (i + 1 >= argc) usage_error(argv[i]);
            out->host = argv[i + 1];
            i++;
            continue;
        }

        if (strncmp(argv[i], "--host=", 7) == 0) {
            out->host = argv[i] + 7;
            continue;
        }

        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            if (out->pos_count >= ARGS_MAX_POS) break;
            out->pos[out->pos_count++] = argv[i];
            continue;
        }

        if (argv[i][0] == '-' && argv[i][1] != '\0') {
            unknown_opt_error(argv[i]);
        }

        if (out->pos_count >= ARGS_MAX_POS) break;
        out->pos[out->pos_count++] = argv[i];
    }
}
