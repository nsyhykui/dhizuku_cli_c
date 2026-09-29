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

#ifndef DCLI_OUTPUT_H
#define DCLI_OUTPUT_H

/*
 * 按行处理服务端响应，分流到 stdout / stderr。
 * Warning 走 stderr 黄字，错误走 stderr 红字，其他走 stdout。
 * 返回 1 表示存在错误行（退出码应为 1），否则 0。
 */
int output_print(const char *reply);

#endif /* DCLI_OUTPUT_H */
