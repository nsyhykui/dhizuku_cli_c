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

#ifndef DCLI_CONFIG_H
#define DCLI_CONFIG_H

#include <stddef.h>

/*
 * 读取配置字段。
 * 优先级：环境变量 > 当前目录文件 > HOME 目录文件
 * 都找不到返回 NULL。
 * 返回的字符串由 malloc 分配，调用方负责 free。
 */
char *config_read_field(const char *fname, const char *envname);

/*
 * 读取 TOTP 密钥。
 * 返回 malloc 分配的字符串，失败返回 NULL。
 */
char *config_load_key(void);

/*
 * 读取服务端地址。
 * cli_host 是命令行参数，优先级最高。
 * 返回 malloc 分配的字符串；调用方负责 free。
 * 失败时返回默认值 "127.0.0.1" 的副本。
 */
char *config_load_host(const char *cli_host);

#endif /* DCLI_CONFIG_H */
