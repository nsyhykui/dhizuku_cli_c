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

#include <string.h>

#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>

#include "crypto.h"
#include "common.h"

static const unsigned char HKDF_SALT[32] = {0};
static const unsigned char HKDF_INFO[]  = "dhizuku-cli-v1";

int hkdf_derive_aes_key(const unsigned char *ikm, size_t ikm_len,
                        unsigned char *out) {
    unsigned char prk[32];
    unsigned int prk_len = 0;

    if (!HMAC(EVP_sha256(), HKDF_SALT, (int)sizeof(HKDF_SALT),
              ikm, (int)ikm_len, prk, &prk_len)) {
        return -1;
    }

    /* HKDF-Expand：单块输出（32 字节，不超过 HashLen） */
    unsigned char block[32];
    unsigned int block_len = 0;

    HMAC_CTX *ctx = HMAC_CTX_new();
    if (!ctx) return -1;

    HMAC_Init_ex(ctx, prk, (int)prk_len, EVP_sha256(), NULL);
    HMAC_Update(ctx, HKDF_INFO, sizeof(HKDF_INFO) - 1);
    unsigned char counter = 1;
    HMAC_Update(ctx, &counter, 1);
    HMAC_Final(ctx, block, &block_len);
    HMAC_CTX_free(ctx);

    if (block_len < AES_KEY_LEN) return -1;
    memcpy(out, block, AES_KEY_LEN);
    return 0;
}

int aes_gcm_encrypt(const unsigned char *key, size_t key_len,
                    const unsigned char *plain, size_t plain_len,
                    unsigned char *out, size_t *out_len) {
    (void)key_len;

    unsigned char nonce[NONCE_LEN];
    if (RAND_bytes(nonce, NONCE_LEN) != 1) return -1;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    int len = 0;
    size_t total = 0;

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL) != 1)
        goto fail;
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, NONCE_LEN, NULL) != 1)
        goto fail;
    if (EVP_EncryptInit_ex(ctx, NULL, NULL, key, nonce) != 1)
        goto fail;

    memcpy(out, nonce, NONCE_LEN);
    total = NONCE_LEN;

    if (EVP_EncryptUpdate(ctx, out + total, &len,
                          plain, (int)plain_len) != 1)
        goto fail;
    total += (size_t)len;

    if (EVP_EncryptFinal_ex(ctx, out + total, &len) != 1)
        goto fail;
    total += (size_t)len;

    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAG_LEN,
                            out + total) != 1)
        goto fail;
    total += TAG_LEN;

    EVP_CIPHER_CTX_free(ctx);
    *out_len = total;
    return 0;

fail:
    EVP_CIPHER_CTX_free(ctx);
    return -1;
}
