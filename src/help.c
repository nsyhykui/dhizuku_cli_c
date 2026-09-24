/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "help.h"
#include "i18n.h"

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
    printf("%s\n", tr("  --                   Stop option parsing",
                      "  --                   停止解析后续选项"));
    printf("\n%s\n", tr("Available commands:", "可用命令:"));
    for (size_t i = 0; i < HELP_COUNT; i++) {
        printf("  %-18s %s\n", HELP[i].name,
               tr(HELP[i].desc_en, HELP[i].desc_zh));
    }
    printf("\n%s\n", tr("Remote examples:", "远程连接示例:"));
    printf("  dcli --host 192.168.1.100 ping\n");
    printf("%s export DCLI_HOST=192.168.1.100\n",
           tr("  or env:", "  或环境变量:"));
    printf("%s echo 192.168.1.100 > ~/.dcli_host\n",
           tr("  or file:", "  或写入文件:"));
    printf("\n%s\n", tr("Examples:", "示例:"));
    printf("  dcli ping\n");
    printf("  dcli lock_now\n");
    printf("  dcli hide com.example.app\n");
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
    printf("%s %s\n", tr("Unknown command:", "未知命令:"), cmd);
    exit(1);
}
