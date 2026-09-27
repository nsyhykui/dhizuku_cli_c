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

#ifndef DCLI_ARGS_H
#define DCLI_ARGS_H

#define ARGS_MAX_POS 128

struct parsed_args {
    char *host;                     /* --host / -H 的值，可能为 NULL */
    const char *pos[ARGS_MAX_POS];  /* 位置参数（第一个通常是命令名） */
    int pos_count;
    int is_version;                 /* 1 表示用户传了 --version / -V */
};

/*
 * 解析命令行参数。
 * 只解析第一个位置参数之前的选项。遇到第一个非选项后，
 * 剩余所有参数（包括 --xxx）都当成位置参数，交给服务端处理。
 * 出错时直接 exit(2)。
 */
void args_parse(int argc, char **argv, struct parsed_args *out);

#endif /* DCLI_ARGS_H */
