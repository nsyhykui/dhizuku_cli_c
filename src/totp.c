/*
 * dcli - 通过 TCP 调用 Dhizuku 的 DO 命令工具
 * Copyright (C) 2026 nsyhykui
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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
#include <string.h>
#include <stdint.h>
#include <time.h>

#include <openssl/hmac.h>
#include <openssl/evp.h>

#include "totp.h"

#define TIME_STEP 30

void totp_generate(const char *key, char *out, size_t out_size) {
    uint64_t t = (uint64_t)(time(NULL) / TIME_STEP);

    unsigned char msg[8];
    for (int i = 7; i >= 0; i--) {
        msg[i] = (unsigned char)(t & 0xFF);
        t >>= 8;
    }

    unsigned char hmac[20];
    unsigned int hmac_len = 0;
    HMAC(EVP_sha1(), key, (int)strlen(key), msg, 8, hmac, &hmac_len);

    int off = hmac[19] & 0x0F;
    uint32_t code = ((hmac[off]     & 0x7F) << 24) |
                    ((hmac[off + 1] & 0xFF) << 16) |
                    ((hmac[off + 2] & 0xFF) << 8)  |
                    (hmac[off + 3]  & 0xFF);
    code %= 1000000;

    snprintf(out, out_size, "%06u", code);
}
