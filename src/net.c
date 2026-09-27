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
#include <errno.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "net.h"
#include "common.h"
#include "i18n.h"
#include "totp.h"
#include "crypto.h"
#include "base64.h"

static ssize_t send_all(int fd, const char *buf, size_t len) {
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = send(fd, buf + sent, len - sent, 0);
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) break;
        sent += (size_t)n;
    }
    return (ssize_t)sent;
}

static ssize_t recv_some(int fd, char *buf, size_t len) {
    ssize_t n;
    do {
        n = recv(fd, buf, len, 0);
    } while (n < 0 && errno == EINTR);
    return n;
}

int net_send_command(const char *host, const char *key,
                     const char *cmd_line,
                     char *reply, size_t reply_size,
                     int timeout_sec) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        snprintf(reply, reply_size, "%s",
                 tr("Error: socket failed", "错误: socket 失败"));
        return -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFAULT_PORT);

    if (inet_pton(AF_INET, host, &addr.sin_addr) != 1) {
        snprintf(reply, reply_size, "%s %s",
                 tr("Error: invalid IP", "错误: 非法 IP"), host);
        close(sock);
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        snprintf(reply, reply_size, "%s %s:%d",
                 tr("Error: cannot connect to", "错误: 无法连接"),
                 host, DEFAULT_PORT);
        close(sock);
        return -1;
    }

    struct sockaddr_in local;
    socklen_t local_len = sizeof(local);
    getsockname(sock, (struct sockaddr *)&local, &local_len);

    char local_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &local.sin_addr, local_ip, sizeof(local_ip));
    int local_port = ntohs(local.sin_port);
    int uid = (int)getuid();

    char totp[8];
    totp_generate(key, totp, sizeof(totp));

    char plaintext[BUF_SIZE];
    snprintf(plaintext, sizeof(plaintext), "%d %s %d %s %s",
             uid, local_ip, local_port, totp, cmd_line);

    unsigned char aes_key[AES_KEY_LEN];
    if (hkdf_derive_aes_key((unsigned char *)key, strlen(key), aes_key) != 0) {
        snprintf(reply, reply_size, "%s",
                 tr("Error: hkdf failed", "错误: hkdf 失败"));
        close(sock);
        return -1;
    }

    unsigned char encrypted[BUF_SIZE];
    size_t encrypted_len = 0;
    if (aes_gcm_encrypt(aes_key, AES_KEY_LEN,
                        (unsigned char *)plaintext, strlen(plaintext),
                        encrypted, &encrypted_len) != 0) {
        snprintf(reply, reply_size, "%s",
                 tr("Error: encrypt failed", "错误: 加密失败"));
        close(sock);
        return -1;
    }

    char b64[BUF_SIZE * 2];
    int b64_len = b64_encode(encrypted, encrypted_len, b64, sizeof(b64));
    if (b64_len < 0) {
        snprintf(reply, reply_size, "%s",
                 tr("Error: base64 failed", "错误: base64 失败"));
        close(sock);
        return -1;
    }
    b64[b64_len++] = '\n';
    b64[b64_len] = 0;

    if (send_all(sock, b64, (size_t)b64_len) < 0) {
        snprintf(reply, reply_size, "%s",
                 tr("Error: send failed", "错误: 发送失败"));
        close(sock);
        return -1;
    }

    /* 接收超时 */
    struct timeval tv;
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    /* 读到 EOF（不是读到第一个 \n） */
    size_t total = 0;
    while (total < reply_size - 1) {
        ssize_t n = recv_some(sock, reply + total, reply_size - 1 - total);
        if (n <= 0) break;
        total += (size_t)n;
    }
    reply[total] = 0;

    /* 去掉末尾换行 */
    size_t len = strlen(reply);
    while (len > 0 && (reply[len - 1] == '\n' || reply[len - 1] == '\r'))
        reply[--len] = 0;

    close(sock);
    return 0;
}
