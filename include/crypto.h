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

#ifndef DCLI_CRYPTO_H
#define DCLI_CRYPTO_H

#include <stddef.h>

/*
 * HKDF-SHA256 派生 AES-256 密钥。
 * ikm:     共享密钥（TOTP key）
 * ikm_len: 共享密钥长度
 * out:     输出缓冲区，至少 AES_KEY_LEN 字节
 * 返回 0 成功，-1 失败。
 */
int hkdf_derive_aes_key(const unsigned char *ikm, size_t ikm_len,
                        unsigned char *out);

/*
 * AES-256-GCM 加密。
 * key:       密钥（AES_KEY_LEN 字节）
 * key_len:   密钥长度
 * plain:     明文
 * plain_len: 明文长度
 * out:       输出缓冲区，至少 plain_len + NONCE_LEN + TAG_LEN
 * out_len:   实际写入的字节数（nonce || ciphertext || tag）
 * 返回 0 成功，-1 失败。
 */
int aes_gcm_encrypt(const unsigned char *key, size_t key_len,
                    const unsigned char *plain, size_t plain_len,
                    unsigned char *out, size_t *out_len);

#endif /* DCLI_CRYPTO_H */
