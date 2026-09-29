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

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "output.h"
#include "i18n.h"
#include "common.h"

static const char *ERROR_PREFIXES[] = {
    "Failed:", "Error:", "Denied",
    "timeout",
    "crypto: denied",
    "totp: denied",
    "uid: denied",
    NULL
};

static int is_error_line(const char *line) {
    for (int i = 0; ERROR_PREFIXES[i] != NULL; i++) {
        size_t len = strlen(ERROR_PREFIXES[i]);
        if (strncmp(line, ERROR_PREFIXES[i], len) == 0) return 1;
    }
    return 0;
}

int output_print(const char *reply) {
    if (reply == NULL || *reply == 0) return 0;

    int has_error = 0;
    char buf[BUF_SIZE];
    const char *p = reply;

    while (*p) {
        const char *nl = strchr(p, '\n');
        size_t line_len = nl ? (size_t)(nl - p) : strlen(p);
        if (line_len >= sizeof(buf)) line_len = sizeof(buf) - 1;
        memcpy(buf, p, line_len);
        buf[line_len] = 0;

        if (strncmp(buf, "Warning:", 8) == 0) {
            warn_print("%s", buf);
        } else if (is_error_line(buf)) {
            err_print("%s", buf);
            has_error = 1;
        } else {
            printf("%s\n", buf);
        }

        if (!nl) break;
        p = nl + 1;
    }

    return has_error;
}
