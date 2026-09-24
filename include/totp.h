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

#ifndef DCLI_TOTP_H
#define DCLI_TOTP_H

#include <stddef.h>

/*
 * 生成 TOTP 验证码（RFC 6238，HMAC-SHA1，30s 步长，6 位数字）。
 * key:     共享密钥（字符串）
 * out:     输出缓冲区，至少 7 字节
 * out_size: out 的大小
 */
void totp_generate(const char *key, char *out, size_t out_size);

#endif /* DCLI_TOTP_H */
