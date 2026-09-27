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

#ifndef DCLI_HELP_H
#define DCLI_HELP_H

int help_is_known_command(const char *cmd);
void help_print_global(void);
void help_print_command(const char *cmd);

/*
 * 打印版本信息。
 * server_version:
 *   NULL       → 连不上服务端，只显示客户端版本
 *   "unknown"  → 服务端太旧，不认 #$%version
 *   其他字符串  → 服务端版本号
 */
void help_print_version(const char *server_version);

/* status / status permission 的本地用法说明 */
void help_print_status(void);
void help_print_status_permission(void);

#endif /* DCLI_HELP_H */
