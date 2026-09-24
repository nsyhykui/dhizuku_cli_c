/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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

#include "config.h"
#include "common.h"

char *config_read_field(const char *fname, const char *envname) {
    if (envname) {
        const char *env = getenv(envname);
        if (env && *env) return strdup(env);
    }

    const char *home = getenv("HOME");
    char path[512];

    FILE *fp = fopen(fname, "r");
    if (!fp && home) {
        snprintf(path, sizeof(path), "%s/%s", home, fname);
        fp = fopen(path, "r");
    }
    if (!fp) return NULL;

    char buf[512];
    if (!fgets(buf, sizeof(buf), fp)) {
        fclose(fp);
        return NULL;
    }
    fclose(fp);

    size_t len = strlen(buf);
    while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r'))
        buf[--len] = 0;

    if (len == 0) return NULL;
    return strdup(buf);
}

char *config_load_key(void) {
    return config_read_field(KEY_FILE, "DCLI_KEY");
}

char *config_load_host(const char *cli_host) {
    if (cli_host && *cli_host) return strdup(cli_host);

    char *from_file = config_read_field(HOST_FILE, "DCLI_HOST");
    if (from_file) return from_file;

    return strdup(DEFAULT_HOST);
}
