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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help.h"
#include "i18n.h"
#include "common.h"

struct cmd_help {
    const char *name;
    const char *desc_en;
    const char *desc_zh;
    const char *usage;
};

static struct cmd_help HELP[] = {
    {"ping",              "Test connection",   "测试连接",       "dcli ping"},
    {"lock_now",          "Lock screen now",   "立即锁屏",       "dcli lock_now"},
    {"hide",              "Hide app",          "隐藏指定应用",   "dcli hide <package>"},
    {"unhide",            "Unhide app",        "取消隐藏应用",   "dcli unhide <package>"},
    {"suspend",           "Suspend app",       "挂起应用",       "dcli suspend <package>"},
    {"resume",            "Resume app",        "恢复挂起",       "dcli resume <package>"},
    {"block_uninstall",   "Block uninstall",   "阻止卸载",       "dcli block_uninstall <package>"},
    {"unblock_uninstall", "Unblock uninstall", "允许卸载",       "dcli unblock_uninstall <package>"},
    {"status",            "Query status",      "查询状态",       "dcli status <subcommand>"},
};

#define HELP_COUNT (sizeof(HELP) / sizeof(HELP[0]))

int help_is_known_command(const char *cmd) {
    for (size_t i = 0; i < HELP_COUNT; i++) {
        if (strcmp(HELP[i].name, cmd) == 0) return 1;
    }
    return 0;
}

void help_print_global(void) {
    printf("dcli - %s\n\n", tr("DO Server CLI client", "DO Server 命令行客户端"));
    printf("%s\n", tr("Usage:", "用法:"));
    printf("  dcli [--host <ip>] help [command]\n");
    printf("  dcli [--host <ip>] <command> [args]\n\n");
    printf("%s\n", tr("Options:", "选项:"));
    printf("%s\n", tr("  --host, -H <ip>      Server IP (default 127.0.0.1)",
                      "  --host, -H <ip>      服务端 IP（默认 127.0.0.1）"));
    printf("%s\n", tr("  --version, -V        Show version",
                      "  --version, -V        显示版本"));
    printf("%s\n", tr("  --help, -h           Show help",
                      "  --help, -h           显示帮助"));
    printf("%s\n", tr("  --                   Stop option parsing",
                      "  --                   停止解析后续选项"));
    printf("\n%s\n", tr("Available commands:", "可用命令:"));
    for (size_t i = 0; i < HELP_COUNT; i++) {
        printf("  %-18s %s\n", HELP[i].name,
               tr(HELP[i].desc_en, HELP[i].desc_zh));
    }
    printf("\n%s\n", tr("Examples:", "示例:"));
    printf("  dcli ping\n");
    printf("  dcli lock_now\n");
    printf("  dcli status hid\n");
    printf("  dcli status permission android.permission.CAMERA\n");
}

void help_print_command(const char *cmd) {
    for (size_t i = 0; i < HELP_COUNT; i++) {
        if (strcmp(HELP[i].name, cmd) == 0) {
            printf("%s %s\n", tr("Usage: ", "用法:  "), HELP[i].usage);
            printf("%s %s\n", tr("Desc:  ", "说明:  "),
                   tr(HELP[i].desc_en, HELP[i].desc_zh));
            return;
        }
    }
    err_print("%s %s", tr("Unknown command:", "未知命令:"), cmd);
    exit(1);
}

void help_print_version(const char *server_version) {
    if (server_version == NULL) {
        printf("dcli %s\n", DCLI_VERSION);
    } else if (strcmp(server_version, "unknown") == 0) {
        printf("dcli %s, %s\n", DCLI_VERSION,
               tr("Dhizuku Cli 2.0.0 or earlier",
                  "Dhizuku Cli 2.0.0 或更早"));
    } else {
        printf("dcli %s, Dhizuku Cli %s\n", DCLI_VERSION, server_version);
    }

    printf("%s\n", tr("Copyright (C) 2026 nsyhykui",
                      "Copyright (C) 2026 nsyhykui"));
    printf("%s\n", tr(
            "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.",
            "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>."));
    printf("%s\n", tr(
            "This is free software: you are free to change and redistribute it.",
            "This is free software: you are free to change and redistribute it."));
    printf("%s\n", tr(
            "There is NO WARRANTY, to the extent permitted by law.",
            "There is NO WARRANTY, to the extent permitted by law."));
    printf("\n");
    printf("%s\n", tr("Written by nsyhykui.", "Written by nsyhykui."));
}

void help_print_status(void) {
    printf("%s\n", tr("Usage: dcli status <subcommand>",
                      "用法: dcli status <子命令>"));
    printf("\n%s\n", tr("Subcommands:", "子命令:"));
    printf("  %-14s %s\n", "hid", tr("List hidden apps",
                                       "列出被隐藏的应用"));
    printf("  %-14s %s\n", "suspend", tr("List suspended apps",
                                           "列出被挂起的应用"));
    printf("  %-14s %s\n", "block_uninstall", tr("List apps with uninstall blocked",
                                                    "列出阻止卸载的应用"));
    printf("  %-14s %s\n", "permission", tr("Query app permissions",
                                              "查询应用权限"));
    printf("\n%s\n", tr("Examples:", "示例:"));
    printf("  dcli status hid\n");
    printf("  dcli status permission android.permission.CAMERA\n");
    printf("  dcli status permission --package com.example.app\n");
}

void help_print_status_permission(void) {
    printf("%s\n", tr("Usage: dcli status permission <subcommand>",
                      "用法: dcli status permission <子命令>"));
    printf("\n%s\n", tr("Subcommands:", "子命令:"));
    printf("  %-14s %s\n", "update", tr("Rescan all apps and update cache",
                                          "重新扫描所有应用并更新缓存"));
    printf("  %-14s %s\n", "<perm>", tr("List apps with this permission",
                                          "列出拥有该权限的应用"));
    printf("  %-14s %s\n", "--package <pkg>",
           tr("List all permissions of this app",
              "列出该应用的所有权限"));
    printf("  %-14s %s\n", "<perm> --package <pkg>",
           tr("Query one app's one permission",
              "查询某应用某权限的状态"));
    printf("\n%s\n", tr("Examples:", "示例:"));
    printf("  dcli status permission update\n");
    printf("  dcli status permission android.permission.CAMERA\n");
    printf("  dcli status permission --package com.example.app\n");
    printf("  dcli status permission android.permission.CAMERA --package com.example.app\n");
}
