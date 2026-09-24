/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DCLI_NET_H
#define DCLI_NET_H

#include <stddef.h>

/*
 * 连接服务端，发送加密命令，接收响应。
 * host:     服务端地址（字符串）
 * key:      TOTP 共享密钥（字符串）
 * cmd_line: 待发送的命令（不含 UID/IP/PORT/TOTP）
 * reply:    输出缓冲区
 * reply_size: reply 的大小
 * 返回 0 成功，-1 失败（失败原因写入 reply）。
 */
int net_send_command(const char *host, const char *key,
                     const char *cmd_line,
                     char *reply, size_t reply_size);

#endif /* DCLI_NET_H */
