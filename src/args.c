/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "i18n.h"

static void missing_arg(const char *a) {
    err_print("%s %s", tr("Error: missing argument after",
                          "错误: 后缺参数"), a);
    exit(2);
}

static void unknown_opt(const char *a) {
    err_print("%s %s", tr("Error: unknown option:",
                          "错误: 未知选项:"), a);
    exit(2);
}

void args_parse(int argc, char **argv, struct parsed_args *out) {
    out->host = NULL;
    out->pos_count = 0;
    out->is_version = 0;

    int parsing_options = 1;

    for (int i = 1; i < argc; i++) {
        if (!parsing_options) {
            if (out->pos_count >= ARGS_MAX_POS) break;
            out->pos[out->pos_count++] = argv[i];
            continue;
        }

        /* 单独的 "-" 当成位置参数 */
        if (argv[i][0] != '-' || argv[i][1] == '\0') {
            parsing_options = 0;
            if (out->pos_count >= ARGS_MAX_POS) break;
            out->pos[out->pos_count++] = argv[i];
            continue;
        }

        if (strcmp(argv[i], "--") == 0) {
            parsing_options = 0;
            continue;
        }

        if (strcmp(argv[i], "--host") == 0 || strcmp(argv[i], "-H") == 0) {
            if (i + 1 >= argc) missing_arg(argv[i]);
            out->host = argv[i + 1];
            i++;
            continue;
        }

        if (strncmp(argv[i], "--host=", 7) == 0) {
            out->host = argv[i] + 7;
            continue;
        }

        if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0) {
            out->is_version = 1;
            continue;
        }

        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            if (out->pos_count >= ARGS_MAX_POS) break;
            out->pos[out->pos_count++] = argv[i];
            continue;
        }

        unknown_opt(argv[i]);
    }
}
