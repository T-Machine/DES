#ifndef _DES_
#define _DES_
#include <string.h>
#include <iostream>
using namespace std;

class DES {
public:
    static string DES_encrypt(string input, bool key[64]);
    static string DES_decrypt(string input, bool key[64]);
};

#endif