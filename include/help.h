/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef DCLI_HELP_H
#define DCLI_HELP_H

int help_is_known_command(const char *cmd);
void help_print_global(void);
void help_print_command(const char *cmd);

#endif /* DCLI_HELP_H */
