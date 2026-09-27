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

#ifndef DCLI_NET_H
#define DCLI_NET_H

#include <stddef.h>

/*
 * 连接服务端，发送加密命令，接收响应（读到 EOF）。
 *
 * timeout_sec:
 *   接收超时秒数。版本查询用 3，普通命令用 65。
 *
 * 返回 0 成功，-1 失败（失败原因写入 reply）。
 */
int net_send_command(const char *host, const char *key,
                     const char *cmd_line,
                     char *reply, size_t reply_size,
                     int timeout_sec);

#endif /* DCLI_NET_H */
