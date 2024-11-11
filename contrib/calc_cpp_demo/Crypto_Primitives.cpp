
extern "C" {
#include "Crypto_Primitives.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

}
#include <iostream>
#include <cstring>
#include <memory>
//int Crypto_Primitives::sym_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
//                                   unsigned char *iv, unsigned char *ciphertext)
//{
//    EVP_CIPHER_CTX *ctx;
//
//    int len;
//
//    int ciphertext_len;
//
//    /* Create and initialise the context */
//    if (!(ctx = EVP_CIPHER_CTX_new()))
//        std::cout << "create cipher ctx failed" << std::endl;
//
//    /*
//     * Initialise the encryption operation. IMPORTANT - ensure you use a key
//     * and IV size appropriate for your cipher
//     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
//     * IV size for *most* modes is the same as the block size. For AES this
//     * is 128 bits
//     */
//    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
//        std::cout << "encrypt init failed" << std::endl;
//
//    /*
//     * Provide the message to be encrypted, and obtain the encrypted output.
//     * EVP_EncryptUpdate can be called multiple times if necessary
//     */
//    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
//        std::cout << "encrypt update failed" << std::endl;
//    ciphertext_len = len;
//
//    /*
//     * Finalise the encryption. Further ciphertext bytes may be written at
//     * this stage.
//     */
//    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
//        std::cout << "encrypt final failed" << std::endl;
//    ciphertext_len += len;
//
//    /* Clean up */
//    EVP_CIPHER_CTX_free(ctx);
//
//    return ciphertext_len;
//}
//
// 自定义删除器
struct EvpCipherCtxDeleter {
    void operator()(EVP_CIPHER_CTX *ctx) const {
        if (ctx) {
            EVP_CIPHER_CTX_free(ctx);
        }
    }
};
extern "C" int sym_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                                   unsigned char *iv, unsigned char *ciphertext)
{
    int len = 0;
    int ciphertext_len = 0;

    // 使用智能指针管理EVP_CIPHER_CTX
    std::unique_ptr<EVP_CIPHER_CTX, EvpCipherCtxDeleter> ctx(EVP_CIPHER_CTX_new());
    if (!ctx) {
        std::cerr << "创建Cipher上下文失败" << std::endl;
        return -1;
    }

    /* 初始化加密操作 */
    if (1 != EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), NULL, key, iv)) {
        std::cerr << "加密初始化失败" << std::endl;
        return -1;
    }

    /* 提供要加密的数据，并获得加密后的输出 */
    if (1 != EVP_EncryptUpdate(ctx.get(), ciphertext, &len, plaintext, plaintext_len)) {
        std::cerr << "加密更新失败" << std::endl;
        return -1;
    }
    ciphertext_len = len;

    /* 完成加密操作 */
    if (1 != EVP_EncryptFinal_ex(ctx.get(), ciphertext + len, &len)) {
        std::cerr << "加密最终化失败" << std::endl;
        return -1;
    }
    ciphertext_len += len;

    return ciphertext_len;
}

