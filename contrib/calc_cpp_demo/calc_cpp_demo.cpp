//
// Created by 陈志庭 on 24-10-16.
//

extern "C" {
#include "postgres.h"
#include "fmgr.h"
#include "Crypto_Primitives.h"


    // 必须包含 PG_MODULE_MAGIC，以确保模块与 PostgreSQL 版本兼容
    PG_MODULE_MAGIC;

    // 函数声明
    PG_FUNCTION_INFO_V1(test_1);
    PG_FUNCTION_INFO_V1(add_ex);
    PG_FUNCTION_INFO_V1(test_enc);
}
#include <iostream>
#include <sstream>
#include <string>
#include <seal/seal.h>
#include <seal/util/uintcore.h>


using namespace std;
using namespace seal;

extern "C" int hexStringToInt(const std::string& hexStr){
    size_t idx = 0;
    int result;
    result = std::stoi(hexStr, &idx, 16);
    return result;
}
extern "C" std::string encodeHexString(const std::string& str) {
	// 根据默认编码获取字节数组
	const std::string hexString = "0123456789abcdef";
	string sb;
	// 将字节数组中每个字节拆解成2位16进制整数
	for (int i = 0; i < str.length(); i++) {
		sb += hexString.at((str[i] & 0xf0) >> 4);
		sb += hexString.at((str[i] & 0x0f) >> 0);
	}
	return sb;
}
extern "C"  void StringToUchar(string input,unsigned char* output){
    for(int i=0;i<input.size();i++) {
        output[i] = input[i];
    }
}
extern "C" void padding16(string plainText,unsigned char* hexData) {
    int padLength = plainText.size();
    if(plainText.size() % 16 != 0) {
        padLength = plainText.size()-plainText.size() % 16 +16;
    }

    for(int i=0;i<padLength;i++) {
        if(i<plainText.size()) {
            hexData[i] = plainText[i];
        }
        else {
            hexData[i] = '0';
        }
    }

}

extern "C" string getSymmetricEncryption(const string& text){
    string key = "XJTU_ESC_OqHn3xt0CsD1DOlBjkJsc3Q";
    string iv = "XJTU_ESC_10s2YIx";

    int padLength = text.size();
    if(text.size() % 16 != 0) {
        padLength = text.size()-text.size() % 16 +16;
    }


    auto* plain_text = new unsigned char[padLength];
    auto* ciphertext = new unsigned char[padLength + 16];
    auto* key_uc = new unsigned char[key.size()];
    auto* iv_uc = new unsigned char[iv.size()];

    StringToUchar(key,key_uc);
    StringToUchar(iv,iv_uc);
    //StringToUchar(text,plain_text);

    padding16(text,plain_text);
    int cipertext_len = sym_encrypt(plain_text,padLength,key_uc,iv_uc,ciphertext);
    //string cipherStr = UcharToString(ciphertext,cipertext_len);
    string cipherStr = string(reinterpret_cast<const char*>(ciphertext), cipertext_len);


    delete[] plain_text;
    delete[] key_uc;
    delete[] iv_uc;

    return cipherStr;
}
// 定义函数
extern "C" Datum test_1(PG_FUNCTION_ARGS) {
    // 使用 C++ 代码
    std::cout << "Hello from C++ in PostgreSQL extension!" << std::endl;
    seal::EncryptionParameters parms(seal::scheme_type::bfv);

    // 设置多项式的模数（多项式环的大小），必须为2的幂次方
    size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);

    // 设置系数模数
    parms.set_coeff_modulus(seal::CoeffModulus::BFVDefault(poly_modulus_degree));

    // 设置纯文本模数（加密运算的模数）
    parms.set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, 20));

    // 创建SEALContext对象
    seal::SEALContext context(parms);

    // 生成密钥
    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    // 创建加密器、解密器和编码器
    Encryptor encryptor(context, public_key);
    Decryptor decryptor(context, secret_key);
    Evaluator evaluator(context);

    // 要加密的整数
    std::uint64_t value1 = 42;
    std::uint64_t value2 = 32;
    Plaintext x_plain1(seal::util::uint_to_hex_string(&value1, std::size_t(1)));
    Plaintext x_plain2(seal::util::uint_to_hex_string(&value2, std::size_t(1)));

    // 加密
    Ciphertext encrypted_v1,encrypted_v2;
    encryptor.encrypt(x_plain1, encrypted_v1);
    encryptor.encrypt(x_plain2, encrypted_v2);
    //cout << "Encrypted message size: " << encrypted.size() << endl;

    //相加
    Ciphertext encrypted_v3;
    evaluator.add(encrypted_v1,encrypted_v2,encrypted_v3);


    // 解密
    Plaintext decrypted_plaintext;
    decryptor.decrypt(encrypted_v3,decrypted_plaintext);
    int res = hexStringToInt(decrypted_plaintext.to_string());
    // 返回一个整数值
    PG_RETURN_INT32(res);
}

extern "C" Datum add_ex(PG_FUNCTION_ARGS) {
    bytea	   *a1,
               *a2;
    stringstream ss;
    Ciphertext encrypted_a1,
        	   encrypted_a2,
        	   encrypted_res;

    EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 2048;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));
    SEALContext context(parms);
    Evaluator evaluator(context);

    a1 = PG_GETARG_BYTEA_PP(0);
    a2 = PG_GETARG_BYTEA_PP(1);
	uint32 a1_len = VARSIZE_ANY_EXHDR(a1);
    uint32 a2_len = VARSIZE_ANY_EXHDR(a2);

    char *a1_raw = VARDATA_ANY(a1);
    char *a2_raw = VARDATA_ANY(a2);

	string a1_str = {reinterpret_cast<const char*>(a1_raw), a1_len};
    int viewA1_Len = a1_str.size();
    ss << a1_str;
    encrypted_a1.load(context,ss);
    ss.str("");
    ss.write(a2_raw, a2_len);
    encrypted_a2.load(context,ss);
	ss.str("");
    evaluator.add(encrypted_a1,encrypted_a2,encrypted_res);
//
    encrypted_res.save(ss);
	uint32 res_len = ss.str().size();
    auto x = ss.str();
	const char* res_c = x.c_str();

    bytea *res = (bytea*)palloc(VARHDRSZ + res_len);
    SET_VARSIZE(res, VARHDRSZ + res_len);
	memcpy((void *)VARDATA(res), res_c, res_len);
    PG_RETURN_BYTEA_P(res);
}

extern "C" Datum test_enc(PG_FUNCTION_ARGS) {
  char* pStr = PG_GETARG_CSTRING(0);
  int32 n1 = PG_GETARG_INT32(1);

  uint32 plen = VARSIZE(pStr) - VARHDRSZ;
  char* pData = (char*)palloc(plen+1);
  memcpy(pData, VARDATA(pStr), plen);
  pData[plen] = '\0';


  stringstream ss;
  ss << pData << "," <<n1;


  string res_str = ss.str();
  // 进行对称加密
  string enc_kwi = getSymmetricEncryption(res_str);
  // 转换为 16进制字符串
  unsigned char kwi_char[enc_kwi.size()];
  StringToUchar(enc_kwi,kwi_char);
  string enc_kwi_hex = encodeHexString(enc_kwi);

  const char *res = (char*)palloc(VARHDRSZ + enc_kwi_hex.size());
  SET_VARSIZE(res, VARHDRSZ + enc_kwi_hex.size());
  memcpy((void *)VARDATA(res), enc_kwi_hex.c_str(), enc_kwi_hex.size());


  PG_RETURN_CSTRING(res);
}
