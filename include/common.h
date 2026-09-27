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

#ifndef DCLI_COMMON_H
#define DCLI_COMMON_H

#ifndef DCLI_VERSION
#define DCLI_VERSION "unknown"
#endif

#define DEFAULT_HOST    "127.0.0.1"
#define DEFAULT_PORT    12345
#define KEY_FILE        ".dcli_key"
#define HOST_FILE       ".dcli_host"
#define BUF_SIZE        8192
#define NONCE_LEN       12
#define AES_KEY_LEN     32
#define TAG_LEN         16

#endif /* DCLI_COMMON_H */
