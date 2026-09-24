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

#ifndef DCLI_BASE64_H
#define DCLI_BASE64_H

#include <stddef.h>

/*
 * Base64 编码（无换行，标准字符表 + /）。
 * out 大小至少为 4 * ((in_len + 2) / 3) + 1。
 * 返回写入的字符数（不含终止符），失败返回 -1。
 */
int b64_encode(const unsigned char *in, size_t in_len,
               char *out, size_t out_size);

#endif /* DCLI_BASE64_H */
