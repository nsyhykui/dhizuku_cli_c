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
#include <stdarg.h>
#include <unistd.h>

#include "i18n.h"

static int g_zh = 0;
static int g_color = 0;

void i18n_init(void) {
    const char *lang = getenv("LANG");
    if (lang && (strncmp(lang, "zh", 2) == 0 ||
                 strncmp(lang, "ZH", 2) == 0 ||
                 strncmp(lang, "Zh", 2) == 0)) {
        g_zh = 1;
    }

    if (getenv("NO_COLOR")) {
        g_color = 0;
    } else {
        g_color = isatty(STDERR_FILENO);
    }
}

const char *tr(const char *en, const char *zh) {
    return g_zh ? zh : en;
}

int color_enabled(void) {
    return g_color;
}

static void color_print(const char *color, const char *reset,
                        const char *fmt, va_list ap) {
    if (g_color) fputs(color, stderr);
    vfprintf(stderr, fmt, ap);
    if (g_color) fputs(reset, stderr);
    fputc('\n', stderr);
}

void err_print(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    color_print("\033[31m", "\033[0m", fmt, ap);
    va_end(ap);
}

void warn_print(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    color_print("\033[33m", "\033[0m", fmt, ap);
    va_end(ap);
}
