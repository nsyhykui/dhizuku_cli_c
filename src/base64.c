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

#include "base64.h"

static const char B64_TABLE[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int b64_encode(const unsigned char *in, size_t in_len,
               char *out, size_t out_size) {
    size_t olen = 4 * ((in_len + 2) / 3);
    if (olen + 1 > out_size) return -1;

    size_t i, j = 0;
    for (i = 0; i + 2 < in_len; i += 3) {
        out[j++] = B64_TABLE[in[i] >> 2];
        out[j++] = B64_TABLE[((in[i]     & 0x03) << 4) | (in[i + 1] >> 4)];
        out[j++] = B64_TABLE[((in[i + 1] & 0x0F) << 2) | (in[i + 2] >> 6)];
        out[j++] = B64_TABLE[in[i + 2] & 0x3F];
    }

    if (i < in_len) {
        out[j++] = B64_TABLE[in[i] >> 2];
        if (i + 1 < in_len) {
            out[j++] = B64_TABLE[((in[i] & 0x03) << 4) | (in[i + 1] >> 4)];
            out[j++] = B64_TABLE[(in[i + 1] & 0x0F) << 2];
            out[j++] = '=';
        } else {
            out[j++] = B64_TABLE[(in[i] & 0x03) << 4];
            out[j++] = '=';
            out[j++] = '=';
        }
    }

    out[j] = 0;
    return (int)j;
}
