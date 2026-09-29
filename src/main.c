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
#include "output.h"

#define TIMEOUT_NORMAL  65
#define TIMEOUT_VERSION 3

/* ================= 版本查询 ================= */

static char *query_server_version(const char *host, const char *key) {
    char reply[BUF_SIZE];
    if (net_send_command(host, key, "#$%version",
                         reply, sizeof(reply), TIMEOUT_VERSION) < 0) {
        return NULL;
    }
    if (reply[0] == 0) return NULL;
    if (strcmp(reply, "Unknown") == 0) return strdup("unknown");
    return strdup(reply);
}

/* ================= dcli status ================= */

static int do_status(const char *host, const char *key) {
    char reply[BUF_SIZE];
    int rc = net_send_command(host, key, "ping",
                              reply, sizeof(reply), TIMEOUT_VERSION);

    const char *state;
    if (rc < 0) {
        state = tr("Not running", "未运行");
    } else if (strcmp(reply, "Success") == 0) {
        state = tr("Running (authorized)", "正在运行（已授权）");
    } else if (strcmp(reply, "uid: denied") == 0) {
        state = tr("Running (unauthorized)", "正在运行（未授权）");
    } else if (strcmp(reply, "totp: denied") == 0) {
        state = tr("Running (TOTP failed)", "正在运行（TOTP 验证失败）");
    } else if (strcmp(reply, "crypto: denied") == 0) {
        state = tr("Running (crypto failed)", "正在运行（解密失败）");
    } else {
        state = tr("Running (unknown)", "正在运行（状态未知）");
    }

    printf("%s: %s\n", tr("Server status", "服务端状态"), state);
    printf("%s: %s\n", tr("Server IP", "服务端IP"), host);
    printf("%s: %d\n", tr("Server port", "服务端端口"), DEFAULT_PORT);
    printf("%s: TCP\n", tr("Mode", "模式"));

    return 0;
}

/* ================= main ================= */

int main(int argc, char **argv) {
    signal(SIGPIPE, SIG_IGN);
    i18n_init();

    struct parsed_args pa;
    args_parse(argc, argv, &pa);

    /* --version */
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

    /* 顶层命令无子命令 → 本地帮助 */
    if (pa.pos_count == 1) {
        if (strcmp(pa.pos[0], "list") == 0)  { help_print_list();  return 0; }
        if (strcmp(pa.pos[0], "pm") == 0)    { help_print_pm();    return 0; }
        if (strcmp(pa.pos[0], "cache") == 0) { help_print_cache(); return 0; }
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

    char *host = config_load_host(pa.host);

    /* dcli status → 本地处理 */
    if (pa.pos_count == 1 && strcmp(pa.pos[0], "status") == 0) {
        int rc = do_status(host, key);
        free(key);
        free(host);
        return rc;
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

    return output_print(reply) ? 1 : 0;
}
