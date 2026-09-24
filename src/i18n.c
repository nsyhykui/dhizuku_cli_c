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

#include <string.h>
#include <stdlib.h>

#include "i18n.h"

static int g_zh = 0;

void i18n_init(void) {
    const char *lang = getenv("LANG");
    if (lang && (strncmp(lang, "zh", 2) == 0 ||
                 strncmp(lang, "ZH", 2) == 0 ||
                 strncmp(lang, "Zh", 2) == 0)) {
        g_zh = 1;
    }
}

const char *tr(const char *en, const char *zh) {
    return g_zh ? zh : en;
}
