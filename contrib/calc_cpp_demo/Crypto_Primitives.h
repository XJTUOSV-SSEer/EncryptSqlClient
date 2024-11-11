#ifndef CRYPTO_PRIMITIVES_H
#define CRYPTO_PRIMITIVES_H


/*
    密码学原语实现
*/
int sym_encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
                                   unsigned char *iv, unsigned char *ciphertext);


#endif