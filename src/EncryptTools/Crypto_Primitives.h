#ifndef CRYPTO_PRIMITIVES_H
#define CRYPTO_PRIMITIVES_H
#include <string>
#include <cstring>

/*
    密码学原语实现
*/
class Crypto_Primitives{
public:
    /*
        对称加密（AES256-cbc模式）
        param:
            plaintext：要加密的数据
            plaintext_len：数据的字节数
            key：密钥，256位
            iv：随机的IV，128位
            ciphertext：密文（调用前需要分配空间，且为16字节的整数倍）
        return:
            密文的实际长度（不算填充）
    */
    static int sym_encrypt(unsigned char* plaintext, int plaintext_len, unsigned char* key,
            unsigned char* iv, unsigned char *ciphertext);

    /*
        解密（AES256-cbc模式）
        param:
            ciphertext：密文
            ciphertext_len：密文的实际长度（即sym_encrypt的返回值）
            key：密钥，256位
            iv：随机的IV，128位
            plaintext：解密得到的明文
        return:
            明文的实际长度
    */
    static int sym_decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key,
            unsigned char* iv, unsigned char* plaintext);

    
    /*
        生成一个num字节长的随机数
        param:
            随机数的字节数
        return:
            结果（转换为std::string）
    */
   static std::string get_rand(int num);


    /*
        PRF函数。调用AES256计算prf(k,data)
        param:
            k：密钥
            data:数据
            len：数据的字节数
            v: 函数值（需要预留空间，且为16字节的整数倍）
        return:
            none
    */
    static int get_prf(unsigned char* k,unsigned char* data,int len, unsigned char* v);

    /*
        对相同长度的字符串进行异或操作（通过按字节异或）
        param:
        a,b：等长的字符串
        len:字节数
        res：结果，需要预先分配空间
    */
    static void string_xor(char* a,char* b,int len,char* res);




    /*
        用于密码学的std::string转换为char[]的方法
        c_str存在问题：若string中某一位为\0，那么转换时就会在这里截断，而在对得到的字节数组进行处理就会内存泄漏

        param:
            c:需要预先分配内存
    */
    static void string2char(std::string str, unsigned char *c);

    /*
        调用SHA512哈希求摘要。
        param:
            message
            message_len
            digest
            digest_len
    */
    static void SHA512_digest(const unsigned char *message, size_t message_len, 
    unsigned char* digest, unsigned int *digest_len);
};



#endif