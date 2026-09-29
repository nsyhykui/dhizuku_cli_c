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
};

static struct cmd_help HELP[] = {
    {"ping",              "Test connection",       "测试连接"},
    {"lock_now",          "Lock screen now",       "立即锁屏"},
    {"hide",              "Hide app",              "隐藏应用"},
    {"unhide",            "Unhide app",            "取消隐藏"},
    {"suspend",           "Suspend app",           "挂起应用"},
    {"resume",            "Resume app",            "恢复挂起"},
    {"block_uninstall",   "Block uninstall",       "阻止卸载"},
    {"unblock_uninstall", "Unblock uninstall",     "允许卸载"},
    {"status",            "Show server status",    "显示服务端状态"},
    {"list",              "List hidden/suspended/blocked apps",
                          "列出隐藏/挂起/阻止卸载的应用"},
    {"pm",                "Package manager operations (pm-style)",
                          "包管理操作（pm 风格）"},
    {"cache",             "Cache operations",      "缓存操作"},
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
    printf("  dcli list hidden\n");
    printf("  dcli pm list packages -3\n");
    printf("  dcli pm list permissions android.permission.CAMERA\n");
}

void help_print_command(const char *cmd) {
    if (strcmp(cmd, "status") == 0) { help_print_status(); return; }
    if (strcmp(cmd, "list") == 0)   { help_print_list();   return; }
    if (strcmp(cmd, "pm") == 0)     { help_print_pm();     return; }
    if (strcmp(cmd, "cache") == 0)  { help_print_cache();  return; }

    for (size_t i = 0; i < HELP_COUNT; i++) {
        if (strcmp(HELP[i].name, cmd) == 0) {
            printf("%s %s\n", tr("Desc:", "说明:"),
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

    printf("%s\n", "Copyright (C) 2026 nsyhykui");
    printf("%s\n", "License GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.");
    printf("%s\n", "This is free software: you are free to change and redistribute it.");
    printf("%s\n", "There is NO WARRANTY, to the extent permitted by law.");
    printf("\n");
    printf("%s\n", "Written by nsyhykui.");
}

void help_print_status(void) {
    printf("%s\n", tr("Usage: dcli status", "用法: dcli status"));
    printf("%s\n", tr("Show server running status, IP, port and mode.",
                      "显示服务端运行状态、IP、端口和模式。"));
}

void help_print_list(void) {
    printf("%s\n", tr("Usage: dcli list <subcommand>",
                      "用法: dcli list <子命令>"));
    printf("\n%s\n", tr("Subcommands:", "子命令:"));
    printf("  %-12s %s\n", "hidden",
           tr("List hidden apps", "列出被隐藏的应用"));
    printf("  %-12s %s\n", "suspended",
           tr("List suspended apps", "列出被挂起的应用"));
    printf("  %-12s %s\n", "blocked",
           tr("List apps with uninstall blocked", "列出阻止卸载的应用"));
}

void help_print_pm(void) {
    printf("%s\n", tr("Usage: dcli pm <subcommand>",
                      "用法: dcli pm <子命令>"));
    printf("\n%s\n", tr("Subcommands:", "子命令:"));
    printf("  %-12s %s\n", "list packages",
           tr("List packages (same as pm list packages)",
              "列出应用（同 pm list packages）"));
    printf("  %-12s %s\n", "list permissions",
           tr("Query app permission state",
              "查询应用权限状态"));
    printf("  %-12s %s\n", "grant",
           tr("Grant a runtime permission",
              "授予运行时权限"));
    printf("  %-12s %s\n", "revoke",
           tr("Revoke a runtime permission",
              "拒绝运行时权限"));
    printf("  %-12s %s\n", "reset",
           tr("Reset a permission to default",
              "恢复权限到默认状态"));
    printf("\n%s\n", tr("Examples:", "示例:"));
    printf("  dcli pm list packages -3\n");
    printf("  dcli pm list packages -f wechat\n");
    printf("  dcli pm list permissions android.permission.CAMERA\n");
    printf("  dcli pm grant com.example.app android.permission.CAMERA\n");
    printf("  dcli pm revoke com.example.app android.permission.CAMERA\n");
    printf("  dcli pm reset com.example.app android.permission.CAMERA\n");
}

void help_print_cache(void) {
    printf("%s\n", tr("Usage: dcli cache <subcommand>",
                      "用法: dcli cache <子命令>"));
    printf("\n%s\n", tr("Subcommands:", "子命令:"));
    printf("  %-12s %s\n", "update",
           tr("Rescan all apps and update cache",
              "重新扫描所有应用并更新缓存"));
}
