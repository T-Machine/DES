#include"des.h"
#include<string.h>
#include<iostream>
using namespace std;

bool keys[16][48];

//各种表
//IP置换表
int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17,  9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7 };
//IP逆置换表
int IP_[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25 };   
//E扩展置换
int E_table[48] = {
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 
    8, 9, 10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 
    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1 };
//P置换
int P_table[32] = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25 };
//S盒
int S_box[8][4][16] = {
    //S1   
    { { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 },
    { 0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8 },
    { 4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0 },
    { 15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 } },
    //S2
    { { 15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10 },
    { 3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5 },
    { 0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15 },
    { 13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 } },
    //S3
    { { 10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8 },
    { 13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1 },
    { 13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7 },
    { 1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 } },
    //S4
    { { 7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15 },
    { 13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9 },
    { 10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4 },
    { 3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 } },
    //S5
    { { 2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9 },
    { 14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6 },
    { 4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14 },
    { 11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 } },
    //S6
    { { 12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11 },
    { 10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8 },
    { 9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6 },
    { 4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 } },
    //S7
    { { 4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1 },
    { 13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6 },
    { 1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2 },
    { 6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 } },
    //S8
    { { 13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7 },
    { 1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2 },
    { 7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8 },
    { 2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 } } };
//PC_1置换表
int PC_1[56] = {
    57, 49, 41, 33, 25, 17,  9,  1,
    58, 50, 42, 34, 26, 18, 10,  2,
    59, 51, 43, 35, 27, 19, 11,  3,
    60, 52, 44, 36, 63, 55, 47, 39,
    31, 23, 15,  7, 62, 54, 46, 38,
    30, 22, 14,  6, 61, 53, 45, 37,
    29, 21, 13,  5, 28, 20, 12,  4 };
//PC_2压缩置换表
int PC_2[48] = {
    14, 17, 11, 24,  1,  5,  3, 28,
    15,  6, 21, 10, 23, 19, 12,  4,
    26,  8, 16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56,
    34, 53, 46, 42, 50, 36, 29, 32 };
//左移位数
int left_shift[16] = {
    1,  1,  2,  2,  2,  2,  2,  2,
    1,  2,  2,  2,  2,  2,  2,  1 };

//函数声明
void transform(bool *in, bool *out, int *table, int length);
void create_keys(bool key[64]);
string DES_process(string input, bool key[64], bool encrypt);
void T_iteration(bool input[64], bool encrypt);
void Feistel(bool right[32], bool subKey[48]);
void stringTo64(string input, bool output[64]);
string stringFrom64(bool input[64]);
string PKCS5(string str);

//类函数
string DES::DES_encrypt(string input, bool key[64]) {
    return DES_process(input, key, true);
}
string DES::DES_decrypt(string input, bool key[64]) {
    string decrypt = DES_process(input, key, false);
    int count = decrypt[decrypt.length() - 1];
    return decrypt.substr(0, decrypt.length() - count);
}

//函数实现
//置换
void transform(bool *in, bool *out, int *table, int length) {
    for (int i = 0; i < length; ++i) {
        out[i] = in[table[i] - 1];
    }
}

void create_keys(bool key[64]) {
    bool C[56], D[56];///
    //64->56
    for(int i = 0; i < 28; i ++) {
        C[i + 28] = C[i] = key[PC_1[i] - 1];
        D[i + 28] = D[i] = key[PC_1[i + 28] - 1];
    }
    //56->48
    int shift = 0;
    for(int i = 0; i < 16; i ++) {
        shift += left_shift[i];
        for (int j = 0; j < 48; ++j) {
            if (PC_2[j] < 28)
                keys[i][j] = C[PC_2[j] + shift - 1];
            else
                keys[i][j] = D[PC_2[j] - 28 + shift - 1];
        }
    }
}

void Feistel(bool right[32], bool f_result[32], bool subKey[48]) {
    //E扩展
    bool E[48];
    transform(right, E, E_table, 48);
    //异或
    for(int i = 0; i < 48; i ++) {
        E[i] ^= subKey[i];
    }
    //S-box
    bool S[32] = {0};
    for(int i = 0; i < 8; i ++) {
        int index = i * 6;
        int row = E[i] * 2 + E[i + 5];
        int col = E[i+1]*8 + E[i+2]*4 + E[i+3]*2 + E[i+4];
        int Si = S_box[i][row][col];
        for(int j = 0; j < 4; j ++) {
            S[i*4 + 3 - j]  = Si % 2;
            Si >>= 1;
        }
    }
    //P置换
    transform(S, f_result, P_table, 32);
}

void T_iteration(bool input[64], bool encrypt) {
    for(int i = 0; i < 16; i ++) {
        bool next[64], right[32], left[32], f_result[32];
        for(int j = 0; j < 32; j ++) {
            next[j] = input[32 + j];    //Li = Ri-1
            left[j] = input[j];         //Li-1
            right[j] = input[32 + j];   //Ri-1
        }
        //轮函数
        if(encrypt) {
            Feistel(right, f_result, keys[i]);
        }
        else {
            Feistel(right, f_result, keys[15 - i]);
        }
        //异或
        for(int j = 0; j < 32; j ++) {
            next[32 + j] = left[j] ^ f_result[j];
        }
        //迭代
        for(int j = 0; j < 64; j ++) {
            input[j] = next[j];
        }
    }
    //交换置换W
    bool copy[64];
    for(int i = 0; i < 64; i ++) {
        copy[i] = input[i];
    }
    for(int i = 0; i < 32; i ++) {
        input[i] = copy[32 + i];
        input[32 + i] = copy[i];
    }
}

void stringTo64(string input, bool output[64]) {
    for(int i = 0; i < 8; i ++) {
        int ch = (int)input[i];
        for(int j = 0; j < 8; j ++) {
            output[i*8 + 7 - j] = ch % 2;
            ch >>= 1;
        }
    }
}

string stringFrom64(bool input[64]) {
    string output = "";
    for(int i = 0; i < 8; i ++) {
        int ascii = input[i*8] * 128 + input[i*8 + 1] * 64
                  + input[i*8 + 2] * 32 + input[i*8 + 3] * 16
                  + input[i*8 + 4] * 8 + input[i*8 + 5] * 4
                  + input[i*8 + 6] * 2 + input[i*8 + 7];
        output += ascii;    
    }
    return output;
}

string PKCS5(string str) {
    int len = str.length();
    int count = 8 - len % 8;
    for(int i = 0; i < count; i ++) {
        str += count;
    }
    return str;
}

//执行加密/解密的主函数
string DES_process(string input, bool key[64], bool encrypt) {
    string output = "";
    //生成密钥
    create_keys(key);
    //填充
    if(encrypt) {
        input = PKCS5(input);
    }
    //cout << input << endl;//test
    int num = input.length() / 8; //分段

    for(int i = 0; i < num; i ++) {
        bool segment[64], temp[64], temp2[64];
        stringTo64(input.substr(i*8, 8), segment);
        //初始IP置换
        transform(segment, temp, IP, 64);
        //迭代T && 交换置换W     
        T_iteration(temp, encrypt);
        //逆置换
        transform(temp, temp2, IP_, 64);

        output += stringFrom64(temp2);
        //cout << output << " ";
    }
    return output;
}
