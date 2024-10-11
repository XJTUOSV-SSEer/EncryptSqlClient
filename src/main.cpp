#include "main.h"

#include <iostream>
#include <libpq-fe.h>
#include <string>

#include "DataMapper.h"
#include "EncryptManager.h"
#include "DO/EncKey.h"
#include "DO/EncryptedMultiMap.h"
#include "EncryptTools/Crypto_Primitives.h"
#include "EncryptTools/EncryptUtil.h"
#include "seal/seal.h"

#include<gmpxx.h>
#include<gmp.h>

#include <pybind11/embed.h> // 用于嵌入 Python 解释器
namespace py = pybind11;
using namespace std;
using namespace seal;

void testPaillier() {
    clock_t time=clock();
	 gmp_randstate_t grt;
	 gmp_randinit_default(grt);
	 gmp_randseed_ui(grt, time);

	 //p、q初始化
	 mpz_t p,q,p1,q1;

	 mpz_init(p);
	 mpz_init(q);
	 mpz_init(p1);
	 mpz_init(q1);

	 //p、q的的范围在0~2^128-1
	 mpz_urandomb(p, grt, 128);
	 mpz_urandomb(q, grt, 128);

	  //生成p,q大素数
	 mpz_nextprime(p, p);
	 mpz_nextprime(q, q);

	 //求p，q的乘积 n,以及n的平方n2
	 mpz_t n,n2;

	 mpz_init(n);
	 mpz_init(n2);
	 mpz_mul(n,p,q);
	 mpz_mul(n2,n,n);

	 //设置g,取值g=n+1
	 mpz_t g,j;

	 mpz_init(g);
	 mpz_init_set_ui(j,1);
	 mpz_urandomb(g, grt, 128);
	 //mpz_add(g,n,j);

	 //设置明文m
	 mpz_t m,m1;
	 mpz_init_set_str(m,"22",10);
	 mpz_init_set_str(m1,"33",10);
	 mpz_t r;//设置r,r为随机数
	 mpz_urandomb(r, grt, 128);

	 //设置密文c,c1,需要对这两个密文做同态加法
	 mpz_t c,c1;

	 mpz_init(c);
	 mpz_init(c1);
	 //设置密文c

	 mpz_powm(c,g,m,n2);
	 mpz_powm(r,r,n,n2);
	 mpz_mul(c,c,r);
	 mpz_mod(c,c,n2);

	 //设置密文c1
	 mpz_powm(c1,g,m1,n2);
	 mpz_mul(c1,c1,r);
	 mpz_mod(c1,c1,n2);

	 //解密过程
	 //先求λ，是p、q的最小公倍数,y3代表λ
	 mpz_t y1,y2,y3;

	 mpz_init(y1);
	 mpz_init(y2);
	 mpz_init(y3);

	 mpz_sub(p1,p,j);
	 mpz_sub(q1,q,j);
	 mpz_lcm(y3,p1,q1);//y3代表λ

	 //输出明文m,g
	 //十进制输出是%Zd,十六进制输出是%ZX,folat使用&Ff
	 //gmp_printf("p = %Zd\n\n", p);
	 //gmp_printf("q = %Zd\n\n", q);
	 //gmp_printf("r = %Zd\n\n", r);
	 //gmp_printf("g = %Zd\n\n", g);
	 //gmp_printf("λ = %Zd\n\n", y3);
	 //输出密文
	 gmp_printf("明文m = %Zd\n\n", m);
	 gmp_printf("密文c = %Zd\n\n",c);
	 gmp_printf("明文m1 = %Zd\n\n", m1);
	 gmp_printf("密文c = %Zd\n\n",c1);

	 //两个密文做同态加法:密文做乘法，最后解密是明文做加法
	 mpz_mul(c,c,c1);
	 mpz_mod(c,c,n2);

	 //y1代表c的λ次方摸n平方
	 mpz_powm(y1,c,y3,n2);
	 mpz_sub(y1,y1,j);
	 mpz_div(y1,y1,n);

	 //y2代表g的λ次方摸n平方
	 mpz_powm(y2,g,y3,n2);
	 mpz_sub(y2,y2,j);
	 mpz_div(y2,y2,n);

	 mpz_t x_y;
	 mpz_init(x_y);
	 mpz_invert(x_y,y2,n);//至关重要的一步，取逆

	 mpz_mul(x_y,x_y,y1);
	 mpz_mod(x_y,x_y,n);
	 //输出明文
	 gmp_printf("解密得到明文m = %Zd\n\n",x_y);
	 mpz_clear(p);
	 mpz_clear(q);
	 mpz_clear(n);
	 mpz_clear(n2);
	 mpz_clear(p1);
	 mpz_clear(q1);
	 mpz_clear(c);
	 mpz_clear(g);
	 mpz_clear(j);
	 mpz_clear(r);
	 mpz_clear(m);
	 mpz_clear(y2);
	 mpz_clear(y1);
	 mpz_clear(y3);
	 mpz_clear(x_y);


}

void testSeal() {
    stringstream ss;

    EncryptionParameters parms(scheme_type::bfv);

    // 设置多项式的模数（多项式环的大小），必须为2的幂次方
    size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);

    // 设置系数模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    // 设置纯文本模数（加密运算的模数）
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

    // 创建SEALContext对象
    SEALContext context(parms);

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
    int value1 = 42;
    int value2 = 32;
    Plaintext x_plain1(uint64_to_hex_string(value1));
    Plaintext x_plain2(uint64_to_hex_string(value2));

    // 加密
    Ciphertext encrypted_v1,encrypted_v2;
    encryptor.encrypt(x_plain1, encrypted_v1);
    encryptor.encrypt(x_plain2, encrypted_v2);
    //cout << "Encrypted message size: " << encrypted.size() << endl;

    //相加
    Ciphertext encrypted_v3;
    evaluator.add(encrypted_v1,encrypted_v2,encrypted_v3);
    encrypted_v3.save(ss);

    string viewSS = ss.str();
    cout << "CipherText's size is " <<viewSS.size() << endl;
    // 解密
    Plaintext decrypted_plaintext;
    decryptor.decrypt(encrypted_v3,decrypted_plaintext);
    int res = hexStringToInt(decrypted_plaintext.to_string());
    // 解码
    cout << "Decrypted value:" << res << '\n' <<endl;


}
// void testPythonScript() {
//	py::module_ script = py::module_::import("pythonScripts.scriptZPaillier"); // 导入 Python 脚本
//	py::object result = script.attr("addInPlaceReturnPlainValue")(5, 3);        // 调用函数
//	int sum = result.cast<int>();                        // 转换结果
//	std::cout << "结果是：" << sum << std::endl;
//}

int main() {
    EncryptionParameters parms(scheme_type::bfv);

    // 设置多项式的模数（多项式环的大小），必须为2的幂次方
    size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);

    // 设置系数模数
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

    // 设置纯文本模数（加密运算的模数）
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 20));

    // 创建SEALContext对象
    SEALContext context(parms);

    // 生成密钥
    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    /*
     * 测试一下加密函数
     */
    //testAES();

    /*
     * 测试一下 Setup 函数
     */
    vector<string> types = {"string","int","string"};
    vector<vector<string>> tables = {{"czt","81","Good"},{"zhg","81","Goode"}};
    RowMultiMap mm = DataMapper::rowMapperConstruct(0,tables,types,public_key);
//
    //EncryptManager encrypt_manager = EncryptManager();
    //EncryptedMultiMap emm = encrypt_manager.setup(mm);
//
    //vector<string> keys = emm.getKeys();
    //for(auto key : keys) {
    //    testInsert(pair<string,string>(key,emm.get(key)));
    //}

    testSeal();
    testPaillier();
	//testPythonScript();


    return 0;
}
