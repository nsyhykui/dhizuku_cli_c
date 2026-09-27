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

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "common.h"
#include "i18n.h"
#include "help.h"
#include "args.h"
#include "config.h"
#include "net.h"

#define TIMEOUT_NORMAL  65
#define TIMEOUT_VERSION 3

/* ================= 版本查询 ================= */

/*
 * 连服务端查版本。
 * 返回 malloc 分配的字符串：版本号 / "unknown" / NULL（连不上）。
 */
static char *query_server_version(const char *host, const char *key) {
    char reply[BUF_SIZE];
    if (net_send_command(host, key, "#$%version",
                         reply, sizeof(reply), TIMEOUT_VERSION) < 0) {
        return NULL;
    }

    if (strncmp(reply, "Success ", 8) == 0) {
        char *v = strdup(reply + 8);
        size_t len = strlen(v);
        while (len > 0 && (v[len - 1] == '\n' || v[len - 1] == '\r'))
            v[--len] = 0;
        return v;
    }
    if (strncmp(reply, "Unknown", 7) == 0) {
        return strdup("unknown");
    }
    return NULL;
}

/* ================= 响应输出 ================= */

/*
 * 按行把服务端响应分流到 stdout / stderr。
 * 返回 1 表示有错误行（退出码应为 1）。
 */
static int print_response(const char *reply) {
    int has_error = 0;
    const char *p = reply;

    while (*p) {
        const char *nl = strchr(p, '\n');
        size_t len = nl ? (size_t)(nl - p) : strlen(p);

        /* 复制当前行到 buf */
        char line[BUF_SIZE];
        if (len >= sizeof(line)) len = sizeof(line) - 1;
        memcpy(line, p, len);
        line[len] = 0;

        if (strncmp(line, "Warning:", 8) == 0) {
            warn_print("%s", line);
        } else if (strncmp(line, "Error:", 6) == 0 ||
                   strncmp(line, "Failed:", 7) == 0 ||
                   strcmp(line, "Denied") == 0 ||
                   strcmp(line, "timeout") == 0 ||
                   strcmp(line, "crypto: denied") == 0 ||
                   strcmp(line, "totp: denied") == 0 ||
                   strcmp(line, "uid: denied") == 0) {
            err_print("%s", line);
            has_error = 1;
        } else if (strcmp(line, "Success") == 0) {
            printf("Success\n");
        } else if (strncmp(line, "Success ", 8) == 0) {
            printf("%s\n", line + 8);
        } else {
            printf("%s\n", line);
        }

        if (!nl) break;
        p = nl + 1;
    }

    return has_error;
}

/* ================= main ================= */

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    i18n_init();

    struct parsed_args pa;
    args_parse(argc, argv, &pa);

    /* dcli --version / dcli -V */
    if (pa.is_version) {
        char *key = config_load_key();
        if (!key) {
            help_print_version(NULL);
            return 0;
        }
        char *host = config_load_host(pa.host);
        char *sv = query_server_version(host, key);
        help_print_version(sv);
        free(sv);
        free(host);
        free(key);
        return 0;
    }

    /* 无参数 → 全局帮助 */
    if (pa.pos_count == 0) {
        help_print_global();
        return 0;
    }

    /* help / --help / -h */
    if (strcmp(pa.pos[0], "help") == 0 ||
        strcmp(pa.pos[0], "--help") == 0 ||
        strcmp(pa.pos[0], "-h") == 0) {
        if (pa.pos_count < 2) help_print_global();
        else help_print_command(pa.pos[1]);
        return 0;
    }

    /* <cmd> --help / <cmd> -h */
    if (pa.pos_count >= 2 &&
        (strcmp(pa.pos[1], "--help") == 0 ||
         strcmp(pa.pos[1], "-h") == 0) &&
        help_is_known_command(pa.pos[0])) {
        help_print_command(pa.pos[0]);
        return 0;
    }

    /* status 本地帮助 */
    if (strcmp(pa.pos[0], "status") == 0) {
        if (pa.pos_count == 1) {
            help_print_status();
            return 0;
        }
        if (pa.pos_count == 2 && strcmp(pa.pos[1], "permission") == 0) {
            help_print_status_permission();
            return 0;
        }
        /* 其他情况（status hid / status permission xxx）发服务端 */
    }

    /* 拼命令行 */
    char cmd_line[BUF_SIZE] = {0};
    for (int i = 0; i < pa.pos_count; i++) {
        if (i > 0) {
            strncat(cmd_line, " ",
                    sizeof(cmd_line) - strlen(cmd_line) - 1);
        }
        strncat(cmd_line, pa.pos[i],
                sizeof(cmd_line) - strlen(cmd_line) - 1);
    }

    /* 读密钥 */
    char *key = config_load_key();
    if (!key) {
        err_print("%s", tr("Error: no key found", "错误: 未找到密钥"));
        err_print("%s", tr("Copy the key from the App to ~/.dcli_key",
                           "请从 App 复制密钥，写入 ~/.dcli_key"));
        err_print("%s", tr("or set env DCLI_KEY",
                           "或设置环境变量 DCLI_KEY"));
        return 1;
    }

    /* 读 host */
    char *host = config_load_host(pa.host);

    /* 发送 */
    char reply[BUF_SIZE];
    int rc = net_send_command(host, key, cmd_line,
                              reply, sizeof(reply), TIMEOUT_NORMAL);

    free(key);
    free(host);

    if (rc < 0) {
        err_print("%s", reply);
        return 1;
    }

    return print_response(reply) ? 1 : 0;
}
